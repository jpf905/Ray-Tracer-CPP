##  **C++ Ray Tracer — *Physically Inspired Rendering from Scratch**

[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![Eigen](https://img.shields.io/badge/Math-Eigen-orange.svg)](https://eigen.tuxfamily.org)
[![OpenMP](https://img.shields.io/badge/Parallel-OpenMP-green.svg)](https://www.openmp.org/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

---

### **Overview**
This project implements a **CPU-based ray tracer written in modern C++17**, featuring recursive light transport, realistic materials, and parallel rendering.  
It demonstrates geometry, vector math, and physically based lighting using **Eigen** for fast linear algebra and **OpenMP** for multithreading.

Rendered images are generated from first principles — simulating rays of light bouncing, reflecting, and refracting through 3D scenes.

---

### **Features**
*	**Physically inspired materials:** diffuse, metal, and glass (refraction via Snell’s Law)
*	 **Anti-aliasing** for smooth edges  
*	**Recursive ray tracing** with depth-limited reflection/refraction  
*	**Eigen-powered math:** high-performance `Vector3d` operations  
*	**OpenMP parallelization:** multi-core CPU rendering  
*	**Simple PPM output** for easy viewing or PNG conversion  
*	**Clean modular CMake project** (builds on macOS, Linux, Windows)

---

