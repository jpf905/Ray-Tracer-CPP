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
		<td style="width:50%; vertical-align: top; padding-right: 12px;">
			
			<h3>📐 Technical Notes</h3>
			<ul>
				<li><b>Ray equation:</b> <code>P(t) = origin + t × direction</code></li>
				<li><b>Intersection:</b> quadratic solution for sphere–ray hits</li>
				<li><b>Reflection:</b> <code>R = V − 2(V·N)N</code></li>
				<li><b>Refraction:</b> Snell’s Law + Schlick approximation</li>
				<li><b>Anti-aliasing:</b> random supersampling per pixel</li>
				<li><b>Gamma:</b> sqrt(color) for perceptual brightness</li>
				<li><b>Parallelism:</b> OpenMP over scanlines</li>
			</ul>
			
		</td>
		<td style="width:50%; vertical-align: top; padding-left: 12px;">
			
			<h3>⚙️ Build &amp; Run</h3>
			<ol>
				<li>Create environment:
					<pre><code>conda env create -f environment.yml
						conda activate raytracer-env</code></pre>
				</li>
				<li>Build:
					<pre><code>mkdir build &amp;&amp; cd build
						cmake ..
						cmake --build . -j</code></pre>
				</li>
				<li>Render:
					<pre><code>./raytracer</code></pre>
				</li>
				<li>View:
					<pre><code>magick ../images/output.ppm ../images/output.png
						open ../images/output.png</code></pre>
				</li>
			</ol>
			
		</td>
	</tr>
</table>
