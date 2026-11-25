# Task 3 — SQL: Top Three Salaries Per Department

## Problem Statement

Given a table `Employee` with columns `id`, `name`, `salary`, and `departmentId`, write a query to find the **top three distinct salaries in each department**.

Return the result table with columns: `department`, `employee`, `salary` (the employee earning that salary).

If a department has fewer than three distinct salaries, return all of them. Order by `department` ascending, then `salary` descending.

**Example**

`Employee`:

| id | name | salary | departmentId |
|----|---------|--------|--------------|
| 1  | Joe     | 85000  | 1            |
| 2  | Henry   | 80000  | 2            |
| 3  | Sam     | 60000  | 2            |
| 4  | Max     | 90000  | 1            |
| 5  | Janet   | 69000  | 1            |
| 6  | Randy   | 85000  | 1            |
| 7  | Will    | 70000  | 1            |

`Department`: `| id | name |` → `1 | IT`, `2 | Sales`

Result:

| department | employee | salary |
|------------|----------|--------|
| IT         | Max      | 90000  |
| IT         | Joe      | 85000  |
| IT         | Randy    | 85000  |
| IT         | Will     | 70000  |
| Sales      | Henry    | 80000  |
| Sales      | Sam      | 60000  |

## Approach

1. Use `DENSE_RANK()` partitioned by department and ordered by salary descending so equal salaries share the same rank.
2. Filter rows with rank `<= 3` — this gives three *distinct* salary values per department (ties included).
3. Join with `Department` for the department name and order the output.

`DENSE_RANK` (not `ROW_NUMBER`) is essential: it keeps tied salaries on the same rank so `85000, 85000` count as one distinct salary.

## Complexity

- Time: `O(n log n)` for the window sort over `n` employee rows.
- Space: `O(n)` for the ranked intermediate result.
