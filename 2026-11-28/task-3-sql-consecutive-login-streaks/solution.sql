-- Task 2: consecutive-day login streaks (gaps & islands), longest streak,
--          and day-over-day retention.
-- Runs on SQLite 3.25+ (window functions). Other engines: replace
-- julianday()/date() with EXTRACT(EPOCH FROM ...)/DATE arithmetic.

CREATE TABLE Login (
    user_id    INT  NOT NULL,
    login_date TEXT NOT NULL,          -- 'YYYY-MM-DD'
    PRIMARY KEY (user_id, login_date)
);

INSERT INTO Login (user_id, login_date) VALUES
    (1, '2026-11-01'), (1, '2026-11-02'), (1, '2026-11-03'),
    (1, '2026-11-05'), (1, '2026-11-06'),
    (2, '2026-11-01'), (2, '2026-11-02'), (2, '2026-11-04'),
    (3, '2026-11-03'),
    (4, '2026-11-01'), (4, '2026-11-02'), (4, '2026-11-03'),
    (4, '2026-11-04'), (4, '2026-11-05'),
    (5, '2026-11-01'), (5, '2026-11-03'), (5, '2026-11-05');

-- ---------------------------------------------------------------------------
-- Q1: every maximal run of consecutive calendar days per user.
--     Gaps & islands: within each user, sort days and subtract the running
--     row number from the numeric date. Consecutive days shrink the gap
--     between date and row number at exactly the same rate, so each island
--     of equal (date - row_number) values is one unbroken streak.
-- ---------------------------------------------------------------------------
WITH days AS (
    SELECT DISTINCT user_id, login_date FROM Login
),
islands AS (
    SELECT user_id,
           login_date,
           CAST(julianday(login_date) AS INTEGER)
               - ROW_NUMBER() OVER (PARTITION BY user_id
                                    ORDER BY login_date) AS grp
    FROM days
)
SELECT user_id,
       MIN(login_date) AS streak_start,
       MAX(login_date) AS streak_end,
       COUNT(*)        AS streak_length
FROM islands
GROUP BY user_id, grp
ORDER BY user_id, streak_start;

-- ---------------------------------------------------------------------------
-- Q2: the single longest streak per user (ties broken by earliest start).
-- ---------------------------------------------------------------------------
WITH days AS (
    SELECT DISTINCT user_id, login_date FROM Login
),
islands AS (
    SELECT user_id,
           login_date,
           CAST(julianday(login_date) AS INTEGER)
               - ROW_NUMBER() OVER (PARTITION BY user_id
                                    ORDER BY login_date) AS grp
    FROM days
),
streaks AS (
    SELECT user_id,
           MIN(login_date) AS streak_start,
           MAX(login_date) AS streak_end,
           COUNT(*)        AS streak_length
    FROM islands
    GROUP BY user_id, grp
)
SELECT user_id, streak_start, streak_end, streak_length
FROM (
    SELECT streaks.*,
           ROW_NUMBER() OVER (PARTITION BY user_id
                              ORDER BY streak_length DESC,
                                       streak_start) AS rn
    FROM streaks
)
WHERE rn = 1
ORDER BY user_id;

-- ---------------------------------------------------------------------------
-- Q3: per date, how many distinct users were active, and how many of them
--     also logged in on the previous calendar day (retention).
-- ---------------------------------------------------------------------------
SELECT a.login_date                        AS day,
       COUNT(*)                            AS active_users,
       COUNT(b.user_id)                    AS retained_from_prev_day
FROM (SELECT DISTINCT user_id, login_date FROM Login) a
LEFT JOIN (SELECT DISTINCT user_id, login_date FROM Login) b
       ON b.user_id = a.user_id
      AND b.login_date = date(a.login_date, '-1 day')
GROUP BY a.login_date
ORDER BY a.login_date;
