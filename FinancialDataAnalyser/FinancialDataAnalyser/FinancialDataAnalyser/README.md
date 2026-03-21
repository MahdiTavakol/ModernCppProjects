# 📊 Financial Data Analyser

A modern C++ application to read, filter, sort, and analyze stock market data from CSV or TXT files. The tool enables quick insights into market trends by computing average prices, standard deviations, and maximum drawdowns.

---

## 🚀 Features

- ✅ Read financial data from `.csv` or `.txt` files
- 🔍 Filter records by:
  - Ticker symbol
  - Specific date
- 📈 Compute statistics:
  - **Average** and **Standard Deviation** of closing prices
  - **Maximum Drawdown** (MDD)
- 📊 Sort records by:
  - `DATE`, `TICKER`, `OPEN`, `HIGH`, `LOW`, `CLOSE`, `VOLUME`
- 💾 Export sorted data to CSV
- 🧪 Simple CLI test via `main.cpp`

---

## 📁 File Structure

<pre>
📁 Project Root
├── ChronoDate.h            # Date wrapper based on std::chrono
├── ChronoDate.cpp
├── FinancialDataRecord.h   # Struct for a single financial data row
├── ReadData.h              # File I/O and CSV/TXT parsing
├── ReadData.cpp
├── FinancialData.h         # Filtering, sorting, and stats logic
├── FinancialData.cpp
└── main.cpp                # (Optional) CLI driver or unit test entry point
</pre>

---

## 📝 Input File Format

Both `.csv` and `.txt` are supported.

### Expected csv format:
```
date,ticker,open,high,low,close,volume
2023-04-03,AMZN,101.23,104.00,100.10,103.20,34500000
2023-04-03,AAPL,161.42,162.74,160.10,161.79,38250000
...
```

- Delimited by commas
- Date format: `YYYY-MM-DD`

### Expected txt format:
```
date ticker open high low close volume
2023-04-03 AMZN 101.23 104.00 100.10 103.20 34500000
2023-04-03 AAPL 161.42 162.74 160.10 161.79 38250000
...
```

- Delimited by space
- Date format: `YYYY-MM-DD`

---

## 📊 Sample Output

For each ticker:
- Printed to stdout:
  - Mean and standard deviation of `close` price
  - Max drawdown
- Exported file:
  - `AAPL_data_sorted_CLOSE.csv`, etc.

---

## 📦 Build Instructions

### Prerequisites
- C++20-compatible compiler (e.g., GCC ≥ 10, Clang ≥ 11, MSVC ≥ 2019)
- CMake (optional but recommended)

### Using g++:
```bash
g++ -std=c++20 -o analyser main.cpp FinancialData.cpp ReadData.cpp ChronoDate.cpp
./analyser
```

---

## 🧩 Concepts Practiced

- `std::ifstream`, `std::stringstream`
- `std::transform`, `std::accumulate`, C++20 `ranges`
- Filtering with lambda + `std::ranges::copy_if`
- Enum-based function dispatch using `std::map`
- `std::unique_ptr`, RAII
- Custom `ChronoDate` class with `std::chrono`

---

## 🧠 Future Improvements

- Date range filtering (start and end)
- JSON/Excel export
- Rolling averages and volatility metrics
- Multithreaded I/O and computations

---

## 👨‍💻 Author

**Mahdi Tavakol**  
Department of Engineering Science, University of Oxford  
✉️ mahdi.tavakol@eng.ox.ac.uk

---

## 📄 License

This project is for educational and research purposes. No commercial license is granted.
