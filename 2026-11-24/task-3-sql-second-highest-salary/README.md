# Task 3 — SQL: Second Highest Salary (and Ranking Queries)

## Problem Statement

Given an `Employee` table, write SQL queries for the following:

**Q1. Second Highest Salary**
Return the second highest distinct salary. If no second highest salary
exists, return `NULL`.

**Q2. Department Top Earners**
For each department, return every employee who earns the highest salary
within that department (handle ties), with columns
`department`, `employee`, `salary`.

**Q3. Salary Rank Within Department**
Return `department`, `employee`, `salary` and a dense rank of salaries
within the department (highest salary = 1), ordered by department then
rank.

### Schema
```sql
CREATE TABLE Employee (
    id     INT PRIMARY KEY,
    name   VARCHAR(50),
    salary INT,
    dept   VARCHAR(50)
);
```

### Sample data
```
id | name    | salary | dept
---+---------+--------+--------
1  | Alice   | 90000  | Sales
2  | Bob     | 80000  | Sales
3  | Carol   | 90000  | Sales
4  | Dave    | 70000  | Eng
5  | Eve     | 95000  | Eng
6  | Frank   | 95000  | Eng
```

### Expected results
- Q1 → `90000` (distinct salaries: 95000, 90000, 80000, 70000)
- Q2 → Sales: Alice 90000, Carol 90000; Eng: Eve 95000, Frank 95000
- Q3 → Eng: Eve 95000 (rank 1), Frank 95000 (rank 1), Dave 70000 (rank 2);
  Sales: Alice 90000 (1), Carol 90000 (1), Bob 80000 (2)

## Approach

- **Q1:** use `DISTINCT` + `LIMIT 1 OFFSET 1` so duplicate top salaries
  are collapsed before offsetting. A nested `SELECT MAX(salary) WHERE
  salary < (SELECT MAX(salary) ...)` also works and is shown as an
  alternative (returns `NULL` automatically when there is no second
  value).
- **Q2:** a correlated anti-join — keep rows for which no higher salary
  exists in the same department. This naturally keeps ties. An
  alternative uses `RANK() OVER (PARTITION BY dept ORDER BY salary DESC)`.
- **Q3:** window function `DENSE_RANK()` partitioned by department —
  ties share a rank and no rank numbers are skipped (unlike `RANK()`).

## Complexity
- **Q1:** O(n log n) for the sort/dedup (or O(n) with an index on
  `salary`).
- **Q2/Q3:** O(n log n) for sorting within partitions; with an index on
  `(dept, salary)` the sort can be avoided.
