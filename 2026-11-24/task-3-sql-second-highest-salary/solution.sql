-- Task 3: Second Highest Salary + department top earners + dense rank
-- Runs on SQLite 3.25+ (window functions), PostgreSQL, MySQL 8+.

CREATE TABLE Employee (
    id     INTEGER PRIMARY KEY,
    name   TEXT    NOT NULL,
    salary INTEGER NOT NULL,
    dept   TEXT    NOT NULL
);

INSERT INTO Employee (id, name, salary, dept) VALUES
    (1, 'Alice', 90000, 'Sales'),
    (2, 'Bob',   80000, 'Sales'),
    (3, 'Carol', 90000, 'Sales'),
    (4, 'Dave',  70000, 'Eng'),
    (5, 'Eve',   95000, 'Eng'),
    (6, 'Frank', 95000, 'Eng');

-- Q1: second highest DISTINCT salary (NULL if it does not exist)
SELECT (
    SELECT DISTINCT salary
    FROM Employee
    ORDER BY salary DESC
    LIMIT 1 OFFSET 1
) AS second_highest_salary;

-- Q1 (alternative, portable form): MAX of all salaries below the top one
SELECT MAX(salary) AS second_highest_salary
FROM Employee
WHERE salary < (SELECT MAX(salary) FROM Employee);

-- Q2: every employee tied for the highest salary in their department
--      (anti-join: no higher salary exists in the same department)
SELECT e.dept   AS department,
       e.name   AS employee,
       e.salary AS salary
FROM Employee e
WHERE NOT EXISTS (
    SELECT 1
    FROM Employee h
    WHERE h.dept = e.dept
      AND h.salary > e.salary
)
ORDER BY e.dept, e.name;

-- Q2 (alternative): window function
-- SELECT dept AS department, name AS employee, salary
-- FROM (
--     SELECT dept, name, salary,
--            RANK() OVER (PARTITION BY dept ORDER BY salary DESC) AS rnk
--     FROM Employee
-- ) t
-- WHERE rnk = 1
-- ORDER BY dept, name;

-- Q3: dense rank of salaries within each department (1 = highest)
SELECT dept    AS department,
       name    AS employee,
       salary,
       DENSE_RANK() OVER (PARTITION BY dept ORDER BY salary DESC) AS salary_rank
FROM Employee
ORDER BY dept, salary_rank, name;
