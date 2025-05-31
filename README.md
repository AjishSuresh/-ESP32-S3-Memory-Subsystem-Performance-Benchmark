# 📊 ESP32-S3 Memory Subsystem Performance Benchmark

Part 3 of the project "Optimizing Energy Consumption and Real-Time Performance in IoT-Driven Embedded Systems. This repository contains benchmark results and analysis for the ESP32-S3 memory subsystem, focusing on sequential vs. random access performance, cache efficiency, and memory latency. The tests were conducted using a custom benchmark code under different configurations to evaluate the impact of CPU frequency scaling (DFS) and memory access patterns.

## 🛠️ Test Configuration
| Parameter               | Specification                     |
|-------------------------|-----------------------------------|
| **Device**              | ESP32-S3-WROOM-1                 |
| **Flash Size**          | 8MB Quad SPI Flash               |
| **PSRAM**               | 2MB Octal SPI PSRAM              |
| **SDK Version**         | ESP-IDF v5.1.2                   |
| **CPU Frequency**       | 240MHz (DFS enabled/disabled)    |
| **Test Patterns**       | Sequential, Random, Block Transfer |
| **Compiler Flags**      | `-O2` optimization, no LTO       |

## 📈 Key Findings

### 🚀 Sequential vs. Random Access Performance
- **Sequential access is 5–7× faster than random access** for reads and **4–5× faster for writes**.
- **Disabling DFS improves performance by 12–22%**, with the largest gains in small random accesses.
- **Larger buffers (64KB–1MB) show more stable performance** due to better prefetching and burst transfers.

### 🧠 Cache Efficiency Metrics
- **L1 Instruction Cache**: 94.2–94.3% hit rate (highly efficient).
- **L1 Data Cache**: 87.6–87.8% hit rate (slightly lower due to varied access patterns).
- **PSRAM Cache**: 76.3–76.5% hit rate (less effective than L1 but still beneficial).

### ⏱️ Memory Latency Analysis
- **SRAM is 3–4× faster than PSRAM and 6–7× faster than Flash**.
- **Disabling DFS reduces latency by 12–15%** across all memory types.
- **PSRAM writes are ~10% slower than reads** due to write buffering delays.

## 💡 Recommendations
1. **Prefer Sequential Over Random Access**  
   - Use contiguous memory structures (e.g., arrays) and block transfers (DMA) where possible.
   
2. **Disable DFS for Latency-Sensitive Tasks**  
   - Lock CPU speed at 240MHz for **12–22% performance improvement** if power consumption is not critical.

3. **Optimize Cache Usage**  
   - Store frequently accessed data in SRAM (use `DRAM_ATTR`).  
   - Align data to 32-bit boundaries for efficient cache-line fills.

4. **Minimize Flash/PSRAM Writes**  
   - Buffer writes in SRAM before committing to Flash/PSRAM to reduce wear and improve speed.

## 📂 Repository Structure
- **`benchmark/`**: Custom benchmark code used for testing( https://github.com/AjishSuresh/-ESP32-S3-Memory-Subsystem-Performance-Benchmark.git)
- **`results/`**: Raw data, tables, and graphs from the tests.
- **`docs/`**: Detailed report and analysis (see `memory.pdf`).

🔧 **Contributions are welcome!** Feel free to open issues or submit pull requests for improvements or additional benchmarks.    
⭐ **If you find this useful, please star the repo!**
