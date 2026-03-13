# RayTracer

An ongoing multithreaded offline ray tracer (CPU-based), written in C++, focusing on rendering, spatial acceleration structures, and multithreaded performance optimization. 

*(Note: This is an active Work in Progress. Current implementation covers high-performance spatial sorting and temporal rendering, with upcoming plans to integrate pure Monte Carlo integration and Probability Density Functions (PDFs).)*

## Current State
![banner](https://github.com/user-attachments/assets/886f830a-5014-4cce-b792-3abeabd31d43)
<div align="center">Figure 1 - Evolution of the Ray Tracer.</div>

---

![RTMB](https://github.com/user-attachments/assets/8f008cc9-c08b-4273-821d-bc377bc1d84c)
<div align="center">Figure 2 - Temporal Rendering (Motion Blur) integrated with BVH and hardware concurrency.</div>

## Performance Benchmarks: Algorithmic vs. Hardware Scaling
The following benchmarks were recorded rendering the high-density Book 1 final scene (approx. 500 primitives) to demonstrate the performance impact of algorithmic spatial sorting compared to raw hardware multithreading.

![RTOWKNDFinal](https://github.com/user-attachments/assets/0652f0c1-b2fe-4eba-b58e-adc1bf963da2)
<div align="center">Figure 3 - High-density benchmarking scene.</div>

---

*Benchmarks recorded on an Intel Core Ultra 7 155H. To maximize performance without being bottlenecked by hardware scheduling, thread count was dynamically set via std::thread::hardware_concurrency. For optimal results on this specific architecture, utilizing the 6 Performance (P) cores is recommended over the Efficiency (E) or Low-Power (LP) cores.)*

| Architecture | Sequential | Multithreaded |
| :--- | :--- | :--- |
| **Linear Intersection - $O(N)$** | 3638.43 seconds (~60 mins) | 499.92 seconds |
| **BVH Traversal - $O(\log N)$** | 450.33 seconds | **84.64 seconds** |

### Key Takeaways:
1. **Algorithmic Optimization > Hardware Brute Force:** A single-threaded BVH implementation (450s) actively outperformed a fully multithreaded brute-force approach (~500s).
2. **Hardware Scaling:** After the $O(\log N)$ spatial optimization, multithreading yielded a highly efficient **~5.3x performance speedup** (~450s down to ~84s).
3. **Total Optimization:** The combination of acceleration structure and hardware concurrency reduced the render time from over an hour to just 1.5 minutes, providing **~43x overall speedup**.

---

## Future Roadmap
* Surface Area Heuristic (SAH) for optimized BVH node splitting.
* Monte Carlo Integration and Importance Sampling.
* Participating Media (Volumetrics/Fog).
* Texture mapping and Perlin noise generation.

## Citations
* Built following the foundational mathematics outlined in Peter Shirley's [*Ray Tracing in One Weekend*](https://raytracing.github.io/books/RayTracingInOneWeekend.html) and [_Ray Tracing: The Next Week_](https://raytracing.github.io/books/RayTracingTheNextWeek.html) books.
