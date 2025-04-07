# Cherenkov Radiation – GasTOF Detector  

---

## 1. Introduction

> 🔍 **Approach:** A gas Cherenkov detector was designed using a C₄F₁₀-filled chamber surrounded by mirrors and a photocathode. The detector tracks emitted photons to infer proton behavior under various simulated conditions.

The detector setup included:
- Protons entering via the front face
- Optional randomness in:
  - Position on the front face
  - Incident angles:  
    - φ: angle across front surface (0°–360°)  
    - θ: off-axis cone deviation
  - Proton energy: 0.01–221 GeV  

The default configuration is a 200 GeV proton, entering centrally and parallel to the x-axis. Each configuration was tested with 2,000 protons.

<p align="center">
  <img src="./img/jeden.png" width="45%">
  <img src="./img/random.png" width="45%">
</p>

<p align="center"><em>Left: Single proton reflection · Right: Multiple proton reflections</em></p>

---

## 2. Results

---

### 2.1 Central parallel beam, Ep = 200 GeV

> 🔍 **Approach:** Default case — central, parallel beam without randomness.

<p align="center">
  <img src="./img/Set.root_hPhotonXZ.png" width="55%"><br>
  <em>Photon impact distribution on the photocathode</em>
</p>

<p align="center">
  <img src="./img/Set.root_hPE_Time.png" width="45%">
  <img src="./img/Set.root_hPhotoElectronsPerEvent.png" width="45%"><br>
  <em>Photoelectron time (left) and count per event (right)</em>
</p>

<p align="center">
  <img src="./img/Set.root_hPhotonEnergy.png" width="45%"><br>
  <em>Incident photon energy distribution</em>
</p>

 **Observations:**
- Photon impact forms a cone centered on the proton hit position
- Photon energy range: **2.8–7.7 eV**
- Photoelectrons mostly emitted ~**1.5 ns**
- Mean number of photoelectrons: **~1100**

---

### 2.2 Parallel beam with random proton position, Ep = 200 GeV

> 🔍 **Approach:** The proton entry position was randomized on the front face (constant direction, energy).

<p align="center">
  <img src="./img/RnadomPosition.root_NumberOfPEvsDistance.png" width="45%">
  <img src="./img/RnadomPosition.root_NumberOfPEvsDistance1D.png" width="45%">
</p>

<p align="center"><em>Photoelectrons vs. distance from beam axis – 2D (left), mean 1D profile (right)</em></p>

 **Findings:**
- Mean photoelectron count remains constant across distance
- Fluctuations increase at greater distances from the center

---

### 2.3 Central beam with random angle of incidence, Ep = 200 GeV

> 🔍 **Approach:** Beam enters at varying φ and θ angles (randomized), fixed position and energy.

<p align="center">
  <img src="./img/RandomAngle.root_NumberOfPEvsPhi.png" width="45%">
  <img src="./img/RandomAngle.root_NumberOfPEvsTheta.png" width="45%">
</p>

<p align="center">
  <img src="./img/RandomAngle.root_NumberOfPEvsAngle.png" width="45%">
</p>

<p align="center"><em>Photoelectrons vs. φ, θ, and full angle</em></p>

 **Findings:**
- **θ** (off-axis tilt) significantly affects generation — smaller θ → more photons
- **φ** has a moderate effect, with peak values in mid-range

---

### 2.4 Central parallel beam with variable energy

> 🔍 **Approach:** Proton energy randomized in 0.01–221 GeV, with fixed entry position and direction.

<p align="center">
  <img src="./img/RandomEnergy.root_NumberOfPEvsProtonEnergy.png" width="45%">
  <img src="./img/RandomEnergy.root_NumberOfPEvsProtonEnergy1D.png" width="45%">
</p>

<p align="center"><em>Photoelectron count vs. energy – 2D (left), mean 1D profile (right)</em></p>

 **Findings:**
- Number of photoelectrons **increases with energy**
- Saturation plateau starts at approx. **30 GeV**

---

## 📌 Summary Table

<p align="center">

<table>
  <tr>
    <th>Scenario</th>
    <th>Randomized</th>
    <th>Key Result</th>
  </tr>
  <tr>
    <td>Default beam</td>
    <td>None</td>
    <td>~1100 photoelectrons at 200 GeV</td>
  </tr>
  <tr>
    <td>Random position</td>
    <td>XY position</td>
    <td>Stable photoelectrons count, larger spread at edges</td>
  </tr>
  <tr>
    <td>Random angle</td>
    <td>φ and θ</td>
    <td>Lower θ → higher PE; φ peaks mid-range</td>
  </tr>
  <tr>
    <td>Random energy</td>
    <td>0.01–221 GeV</td>
    <td>Photoelectron count rises, stabilizes ~30 GeV</td>
  </tr>
</table>

</p>

---

<p align="center"><strong> For simulation scripts or ROOT files, see the project repository.</strong></p>
