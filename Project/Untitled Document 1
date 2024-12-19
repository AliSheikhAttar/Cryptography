## Project Title

### Linear Approximations of SNOW S-boxes

---

### Objective

The project aims to analyze the linear approximation of the substitution box (S-box) used in the SNOW 2.0 algorithm. Specifically, it involves calculating the bias of equality between the input and output masks for the S-box combined with the MixColumn operation.

---

### Key Details

#### 1. S-box Composition

The S-box in SNOW 2.0 consists of two primary components:
- **AES S-box (S\_R(a))**: A cryptographic substitution box that introduces nonlinearity.
- **MixColumn Transformation**: A linear transformation that mixes bytes using a fixed matrix.

#### 2. Linear Approximation

The goal is to compute the bias (deviation from uniform probability) for a given 32-bit mask where the input and output masks are equal. This bias measures the non-random behavior of the S-box.

#### 3. Program Requirements

The program should:
- Take a 32-bit mask as input.
- Compute and return the bias for the given mask.
- Display execution time for performance tracking.

#### 4. Challenges

- Direct computation of the S-box and MixColumn operation is mathematically intensive.
- Efficient computation requires precomputed tables and optimized algorithms.

---

### Mathematical Representation

#### 1. Simplified S-box and MixColumn Relation

For ease of computation, instead of the full matrix-based formula, we use the simplified form:

\[
\begin{pmatrix} r_0 \\ r_1 \\ r_2 \\ r_3 \end{pmatrix} =
S\_R(w_0) \begin{pmatrix} x \\ 1 \\ 1 \\ x+1 \end{pmatrix} +
S\_R(w_1) \begin{pmatrix} x+1 \\ x \\ 1 \\ 1 \end{pmatrix} +
S\_R(w_2) \begin{pmatrix} 1 \\ x+1 \\ x \\ 1 \end{pmatrix} +
S\_R(w_3) \begin{pmatrix} 1 \\ 1 \\ x+1 \\ x \end{pmatrix}
\]

Where:
- \( w_0, w_1, w_2, w_3 \): 8-bit components of the 32-bit input.
- \( r_0, r_1, r_2, r_3 \): 8-bit components of the output.
- \( S\_R(w_i) \): AES S-box applied to input components.
- \( x \): A constant in the finite field \( GF(2^8) \).

This simplified representation reduces the computational complexity compared to the full matrix formulation while maintaining the same result.

#### 2. Precomputed Tables

To simplify computations further, precomputed tables \( T_0(a), T_1(a), T_2(a), T_3(a) \) are defined as:

\[
T_0(a) = \begin{pmatrix} xS\_R(a) \\ S\_R(a) \\ S\_R(a) \\ (x+1)S\_R(a) \end{pmatrix},
T_1(a) = \begin{pmatrix} (x+1)S\_R(a) \\ xS\_R(a) \\ S\_R(a) \\ S\_R(a) \end{pmatrix},
\]

\[
T_2(a) = \begin{pmatrix} S\_R(a) \\ (x+1)S\_R(a) \\ xS\_R(a) \\ S\_R(a) \end{pmatrix},
T_3(a) = \begin{pmatrix} S\_R(a) \\ S\_R(a) \\ (x+1)S\_R(a) \\ xS\_R(a) \end{pmatrix}
\]

Where:
- \( a \): 8-bit input to the S-box.
- \( S\_R(a) \): AES S-box output for input \( a \).
- \( x \) and \( x+1 \): Constants in the MixColumn operation.

These tables allow efficient computation of output components \( r_0, r_1, r_2, r_3 \) without repeatedly applying the MixColumn transformation.

---

### Implementation Notes

#### Bias Calculation

For a given 32-bit mask:
1. Apply the mask to the input.
2. Propagate it through the S-box and MixColumn operations using the precomputed tables.
3. Compare the resulting output to the masked output.
4. Compute the bias as the deviation from uniform probability.

#### Use of Precomputed Tables

The precomputed tables \( T_0(a), T_1(a), T_2(a), T_3(a) \) reduce repeated computations of \( S\_R(a) \) and MixColumn transformations. These tables are stored for all 256 possible values of \( a \) (8-bit input).

#### Performance Tracking

The program should record and display the execution time for bias computation.

---

### Next Steps

1. **Algorithm Design**
   - Develop pseudocode for bias computation using precomputed tables.

2. **Implementation**
   - Write a program to process 32-bit masks, use tables, and compute the bias efficiently.

3. **Validation**
   - Test the program with various input masks to ensure correctness.

4. **Optimization**
   - Explore further optimizations for speed and memory usage.

---

### Purpose of the Project

This project analyzes the cryptographic strength of the SNOW 2.0 S-box combined with the MixColumn operation by evaluating its linear approximation bias. The findings could provide insights into the security of SNOW 2.0 against linear cryptanalysis.


