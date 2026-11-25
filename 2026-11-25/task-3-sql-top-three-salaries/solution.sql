-- Task 3: Top Three Salaries Per Department
-- Schema + sample data + solution (tested on SQLite; DENSE_RANK works in
-- MySQL 8+, PostgreSQL, SQL Server, and SQLite 3.25+).

CREATE TABLE Department (
    id   INTEGER PRIMARY KEY,
    name TEXT NOT NULL
);

CREATE TABLE Employee (
    id           INTEGER PRIMARY KEY,
    name         TEXT NOT NULL,
    salary       INTEGER NOT NULL,
    departmentId INTEGER NOT NULL,
    FOREIGN KEY (departmentId) REFERENCES Department (id)
);

INSERT INTO Department (id, name) VALUES
    (1, 'IT'),
    (2, 'Sales');

INSERT INTO Employee (id, name, salary, departmentId) VALUES
    (1, 'Joe',   85000, 1),
    (2, 'Henry', 80000, 2),
    (3, 'Sam',   60000, 2),
    (4, 'Max',   90000, 1),
    (5, 'Janet', 69000, 1),
    (6, 'Randy', 85000, 1),
    (7, 'Will',  70000, 1);

-- Solution
WITH ranked AS (
    SELECT
        e.name  AS employee,
        e.salary,
        d.name  AS department,
        DENSE_RANK() OVER (
            PARTITION BY e.departmentId
            ORDER BY e.salary DESC
        ) AS rnk
    FROM Employee e
    JOIN Department d ON d.id = e.departmentId
)
SELECT department, employee, salary
FROM ranked
WHERE rnk <= 3
ORDER BY department ASC, salary DESC, employee ASC;
