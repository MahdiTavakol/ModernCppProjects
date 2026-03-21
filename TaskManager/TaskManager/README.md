# 🗂️ Modern Task Manager (Phase 1)

A lightweight command-line application for managing tasks with priorities, due dates, progress tracking, and optional categories — implemented in Modern C++ (C++20).

---

## 🚀 Features

- ✅ Add, remove, and update tasks
- ✅ Sort tasks by priority, due date, or name
- ✅ Mark tasks as completed or finished
- ✅ Highlight high-priority tasks
- ✅ Track tasks ahead or behind schedule
- ✅ Save/load tasks from file
- ✅ Category-based task filtering (basic)
- ✅ Smart memory management via `std::unique_ptr`
- ✅ Uses lambdas for sorting and filtering

---

## 📦 Concepts Demonstrated

| Concept                    | Usage                                  |
|----------------------------|----------------------------------------|
| `std::vector`              | Primary container for tasks            |
| `std::unique_ptr`          | Ownership & RAII of task objects       |
| `std::map`                 | Category support and future extensions |
| Lambda expressions         | Sorting, filtering, and conditionals   |
| RAII                       | File handling, resource management     |
| `std::format`              | Structured terminal output             |
| Custom date class          | `ChronoDate` for robust date handling  |

---

## 🧑‍💻 Sample Output

