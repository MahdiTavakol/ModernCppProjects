# 📊 Financial Data Analyzer

This C++20 project reads historical stock data from `.csv` or `.txt` files, filters records by ticker or date, and computes key financial statistics such as **mean price**, **maximum drawdown**, and **daily return**. Designed to demonstrate practical use of modern C++ concepts like ranges, smart pointers, and chrono-based date management.

---

## 🧩 Concepts Practiced

- `std::ifstream`, `std::stringstream` for robust file I/O
- `std::accumulate`, `std::transform`, `std::ranges::copy_if` for functional data processing
- `std::chrono` and `std::year_month_day` via a custom `ChronoDate` wrapper
- Use of `std::unique_ptr`, RAII, and enum-driven design
- Modular design: parsing, filtering, and analysis are fully decoupled

---

## 🎯 Features

- ✅ **Input Support**: `.csv` and `.txt` file parsing with automatic format detection
- ✅ **Ticker Filtering**: Extract records for a specific stock ticker
- ✅ **Date Filtering**: (planned/optional) Filter records by exact date or date range
- ✅ **Sorting**: Sort data by volume, close price, or other keys using an extensible enum
- ✅ **Statistics**:
  - Average closing price
  - Standard deviation of closing price
  - Maximum drawdown
  - Average daily return

---

## 📂 File Structure
.
├── ChronoDate.h             # Wrapper around std::chrono::year_month_day for date handling
├── ChronoDate.cpp
├── FinancialDataRecord.h    # Struct representing a single stock data entry
├── ReadData.h               # Functions to read and parse .csv or .txt files
├── ReadData.cpp
├── FinancialData.h          # Core logic for filtering, sorting, and computing statistics
├── FinancialData.cpp
└── main.cpp (optional)      # Entry point for CLI or test driver (not included yet)

## 🔧 Build Instructions

Requires a C++20 compatible compiler (e.g. `g++ >= 10`, `clang++ >= 11`, or MSVC with `/std:c++20`).

```bash
g++ -std=c++20 -o analyzer main.cpp ChronoDate.cpp ReadData.cpp FinancialData.cpp
