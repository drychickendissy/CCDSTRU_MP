# CCDSTRU Machine Project  
### Term 2, AY 2024–2025  
### **Due: April 1, 2025 (Tuesday), 08:00**

Implement a computer program (in **C** or **Java**) that satisfies the formal system specification described below. This MP involves sets, relations, and state-based behavior related to a turn-based system operating on subsets of positions.

---

#  Applicable Sets

### **A**
Natural numbers less than 5:  
`A = { x ∈ ℤ⁺ | x < 5 }`

### **P (Positions)**
Cartesian product of `A × A`

### **B (Booleans)**
`B = { true, false }`

### **C (Winning Conditions)**  
A set containing the following subsets of P:

{(1,1),(1,2),(1,3),(1,4)}

{(1,1),(2,2),(3,3),(4,4)}

{(1,4),(2,3),(3,2),(4,1)}

{(4,1),(4,2),(4,3),(4,4)}

### **T**  
A relation on A that is:
- reflexive  
- symmetric  
- antisymmetric  
- transitive  

(meaning T is an equivalence relation, but also antisymmetric, so effectively identity)

---

# System Variables

All of the following are **subsets of P** unless noted otherwise:

- `Uno ⊆ P`
- `Dos ⊆ P`
- `Tres ⊆ P`
- `F ⊆ P`
- `turn ∈ B`
- `go ∈ B`
- `over ∈ B`

---

# System Facts

### **Free positions:**
F = P − (Uno ∪ Tres)

### **Winning sets:**
W = C − T

### **Game over condition:**
over ↔ (Uno ∈ W ∨ Tres ∈ W ∨ F = ∅)

---

# System Initialization

Uno = ∅
Dos = ∅
Tres = ∅
turn = true
go = false

---

# System Behavior

## **NextPlayerMove(pos ∈ P)**

### Case 1: turn = true, go = true, and pos ∈ F  
Uno = Uno ∪ {pos}
turn = ¬turn
go = ¬go

### Case 2: turn = false, and pos ∈ Uno ∪ Tres  
Uno = Uno − {pos}
Tres = Tres − {pos}
turn = ¬turn

### Case 3: turn = true, go = false, and pos ∈ F  
Tres = Tres ∪ {pos}
go = ¬go

---

# GameOver(over)

Return value must be one of:

- **Uno Wins**
- **Dos Wins**
- **Tres Wins**

### Rules:

If Uno ∈ W → Uno Wins
Else if F = ∅ → Dos Wins
Else if Tres ∈ W → Tres Wins


---

# Summary of System Flow

1. Players place or remove elements from subsets Uno and Tres.  
2. `F` always represents positions not yet taken.  
3. `turn` and `go` control which player acts and how.  
4. Game ends when:
   - Uno forms a winning configuration  
   - Tres forms a winning configuration  
   - No free positions remain  
5. The result corresponds to which condition triggered the ending.

---

# Implementation Requirements Summary

You must:

- Implement the system **exactly following the rules** above.  
- Use data structures, functions/methods, and clean code organization.  
- Ensure correctness and determinism of state transitions.  

Language options:
- ✔ **C**
- ✔ **Java**

---

# Machine Project Demo

Link to MP Demo can be found here:
https://drive.google.com/file/d/1QWyCwZNPGgG39AlHlDVp8ZNwUS3yqduZ/view?usp=sharing 
