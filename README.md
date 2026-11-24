# Daily-Tasks

Automated daily coding practice — **2-3 solved tasks** pushed every day at **10:00 PM**.

## Structure

```
YYYY-MM-DD/
├── task-1-<topic>/
│   ├── README.md      # Problem statement
│   └── solution.*     # Working solution
├── task-2-<topic>/
└── task-3-<topic>/
```

## Task Types

- **DSA** — algorithm/data structure problems with code
- **OS** — system programming concepts (processes, scheduling, sync)
- **SQL** — queries, joins, normalization problems

## Automation

- Scheduled task: `DailyGitHubTasks` (Windows Task Scheduler, daily 22:00)
- Script: `opencode run` generates new non-repeating tasks → git commit → push
- Powered by opencode + GulfamRasheed/GitHub
