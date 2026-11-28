# Task 2 — SQL: Employees Earning More Than Their Manager

## Problem Statement

Given a table `Employee` with columns:

| column     | type    | meaning                       |
|------------|---------|-------------------------------|
| `id`       | INT (PK)| employee id                   |
| `name`     | VARCHAR | employee name                 |
| `salary`   | INT     | employee salary               |
| `managerId`| INT     | `id` of the employee's manager (`NULL` if none) |

write a SQL query to find all **employees who earn more than their own
manager**. Return the `employee` name column (aliased `employee`).

**Example**

`Employee`:

| id | name   | salary | managerId |
|----|--------|--------|-----------|
| 1  | Joe    | 70000  | 3         |
| 2  | Henry  | 80000  | 4         |
| 3  | Sam    | 60000  | NULL      |
| 4  | Max    | 90000  | NULL      |

Result:

| employee |
|----------|
| Joe      |

- Joe (70000) reports to Sam (60000) → 70000 > 60000, included.
- Henry (80000) reports to Max (90000) → 80000 ≤ 90000, excluded.
- Sam and Max have no manager → not considered.

## Approach

1. **Self-join** the `Employee` table: the `employee` row joined to the
   `manager` row on `e.managerId = m.id`.
2. Keep only rows where `e.salary > m.salary`.
3. Select `e.name`.

Key points:

- Use an **`INNER JOIN`**, not `LEFT JOIN` — employees with a `NULL`
  `managerId` are unmatched anyway, and the inner join drops them
  naturally.
- `m` and `e` are aliases for the *same* table; the join condition links
  each employee to their manager's row, so both salary columns come from
  the same underlying table.

## Complexity

- With an index on `id` (primary key): `O(n)` for the join/hash probe
  over `n` employee rows.
- Space: `O(n)` for the join intermediate result.
