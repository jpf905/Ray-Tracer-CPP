##  **C++ Ray Tracer — *Physically Inspired Rendering from Scratch**

![Rendered Photo](https://github.com/jpf905/Ray-Tracer-CPP/blob/main/images/output.png)

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

### **Technical Notes**
<table>
	<tr>
		<td width="50%" valign="top">
			
			### 📐 Technical Notes
			| Concept | Description |
			|----------|-------------|
			| **Ray equation** | `P(t) = origin + t * direction` |
			| **Intersection** | Solves quadratic for sphere-ray hits |
			| **Reflection** | `R = V - 2(V·N)N` |
			| **Refraction** | Uses Snell’s Law + Schlick approximation |
			| **Anti-aliasing** | Random supersampling per pixel |
			| **Gamma correction** | √color for realistic brightness |
			| **Parallelism** | OpenMP `#pragma omp parallel for` |