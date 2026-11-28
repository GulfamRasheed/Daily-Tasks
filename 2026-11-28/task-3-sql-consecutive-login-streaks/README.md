# Task 3 — SQL: Consecutive-Day Login Streaks (Gaps & Islands)

## Problem Statement

Given a `Login(user_id, login_date)` table (one row per user per day they
logged in), write SQL queries for the following:

**Q1. All streaks**
For every user, find each *maximal* run of consecutive calendar days they
logged in. Return `user_id`, `streak_start`, `streak_end`, `streak_length`
ordered by user and start date.

**Q2. Longest streak per user**
Return only the single longest streak per user (ties broken by the earliest
start date).

**Q3. Day-over-day retention**
For every date that appears in the table, return `day`, `active_users`
(distinct users that day) and `retained_from_prev_day` (how many of them
also logged in on the previous calendar day), ordered by date.

### Schema
```sql
CREATE TABLE Login (
    user_id    INT  NOT NULL,
    login_date TEXT NOT NULL,          -- 'YYYY-MM-DD'
    PRIMARY KEY (user_id, login_date)
);
```

### Sample data
```
user_id | login_date
--------+------------
1       | 2026-11-01, 11-02, 11-03, 11-05, 11-06
2       | 2026-11-01, 11-02, 11-04
3       | 2026-11-03
4       | 2026-11-01, 11-02, 11-03, 11-04, 11-05
5       | 2026-11-01, 11-03, 11-05
```

### Expected results (verified with SQLite 3.45)
**Q1**
```
user_id | streak_start | streak_end   | streak_length
--------+--------------+--------------+--------------
1       | 2026-11-01   | 2026-11-03   | 3
1       | 2026-11-05   | 2026-11-06   | 2
2       | 2026-11-01   | 2026-11-02   | 2
2       | 2026-11-04   | 2026-11-04   | 1
3       | 2026-11-03   | 2026-11-03   | 1
4       | 2026-11-01   | 2026-11-05   | 5
5       | 2026-11-01   | 2026-11-01   | 1
5       | 2026-11-03   | 2026-11-03   | 1
5       | 2026-11-05   | 2026-11-05   | 1
```

**Q2** (longest per user; user 5 has a 3-way tie → earliest wins)
```
user_id | streak_start | streak_end   | streak_length
1       | 2026-11-01   | 2026-11-03   | 3
2       | 2026-11-01   | 2026-11-02   | 2
3       | 2026-11-03   | 2026-11-03   | 1
4       | 2026-11-01   | 2026-11-05   | 5
5       | 2026-11-01   | 2026-11-01   | 1
```

**Q3**
```
day        | active_users | retained_from_prev_day
2026-11-01 | 4            | 0
2026-11-02 | 3            | 3
2026-11-03 | 4            | 2
2026-11-04 | 2            | 1
2026-11-05 | 3            | 1
2026-11-06 | 1            | 1
```

## Approach

- **Q1 (gaps & islands):** the classic trick. Within each user, order the
  distinct dates and compute
  `grp = CAST(julianday(date) AS INTEGER) - ROW_NUMBER() OVER (PARTITION BY
  user_id ORDER BY date)`.
  For a run of consecutive dates the date increases by exactly 1 per row,
  so `date - row_number` stays **constant**; the first date after a gap
  makes the expression jump. Grouping by `(user_id, grp)` therefore yields
  one row per maximal streak, where `MIN(date)`/`MAX(date)`/`COUNT(*)` give
  start, end and length.
- **Q2:** wrap the Q1 streaks in a CTE and rank them per user with
  `ROW_NUMBER() OVER (PARTITION BY user_id ORDER BY streak_length DESC,
  streak_start)`; keep `rn = 1`. `ROW_NUMBER` (not `RANK`) plus the
  secondary sort key guarantees exactly one row per user on ties.
- **Q3:** a self `LEFT JOIN` of the distinct `(user_id, day)` set to itself
  on `same user_id` and `login_date = date(prev.login_date, '-1 day')`
  (SQLite date arithmetic; use `DATE_SUB`/`INTERVAL` in MySQL or
  `day - INTERVAL '1 day'` in PostgreSQL). `COUNT(*)` counts active users,
  `COUNT(b.user_id)` counts only matched (retained) rows — unmatched left
  joins contribute `NULL`, which `COUNT` skips.

## Complexity

- Let `n` = number of login rows, `d` = distinct user-days.
- **Q1/Q2:** `O(d log d)` for the per-user date sort inside the window
  functions, `O(d)` space for the CTEs; with an index on
  `(user_id, login_date)` the sort can be avoided.
- **Q3:** `O(n)` for building the two distinct sets, `O(n)` hash join
  (or `O(n log n)` without an index on `login_date`), `O(n)` space.
