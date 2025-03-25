# Cherenkov Radiation – GasTOF Detector
## 1. Introduction
The goal of the project was to design a gas Cherenkov radiation detector consisting of a chamber filled with C4F10 gas, surrounded by mirrors and a photocathode. The purpose was to analyze generated photons, and through them, the incoming protons.

Protons always entered through the front face. However, to investigate various scenarios, randomized parameters were implemented: position on the front face, angle of incidence (defined by two angles: φ and θ), and energy (in the range of 0.01 - 221 GeV).

By default, the proton has 200 GeV of energy and travels centrally and parallel to the x-axis. Statistics were gathered for 2000 protons.


– Reflections from a single proton

<p align ="center">
  <img src="./img/jeden.png" width ="500">
</p>

– Reflections from 20 protons

<p align ="center">
  <img src="./img/random.png" width ="500">
</p> 


## 2. Results

### 2.1 Central parallel beam, Ep = 200 GeV

– Photon impact distribution on the photocathode

<p align ="center">
  <img src="./img/Set.root_hPhotonXZ.png" width ="500">
</p> 

– Photoelectron time distribution

<p align ="center">
  <img src="./img/Set.root_hPE_Time.png" width ="500">
</p> 

– Photoelectron count distribution

<p align ="center">
  <img src="./img/Set.root_hPhotoElectronsPerEvent.png" width ="500">
</p> 

– Energy distribution of incident photons

<p align ="center">
  <img src="./img/Set.root_hPhotonEnergy.png" width ="500">
</p> 

Observations:
- The center of the cone (Fig. 3) corresponds to the proton impact point.
- Photon energy range: 2.8 eV – 7.7 eV.
- Most photoelectrons are emitted around 1.5 ns.
- Approximately 1100 photoelectrons are generated on average.

### 2.2 Parallel beam with random proton position on front face, Ep = 200 GeV

– Photoelectron count vs. distance from beam axis

<p align ="center">
  <img src="./img/RnadomPosition.root_NumberOfPEvsDistance.png" width ="500">
</p> 

– Average photoelectron count vs. distance

<p align ="center">
  <img src="./img/RnadomPosition.root_NumberOfPEvsDistance1D.png" width ="500">
</p> 

Findings:
- The average photoelectron count is relatively constant.
- Greater distance leads to greater fluctuations.

### 2.3 Central beam with random angle of incidence, Ep = 200 GeV

– Photoelectrons vs. angle φ

<p align ="center">
  <img src="./img/RandomAngle.root_NumberOfPEvsPhi.png" width ="500">
</p> 

– Photoelectrons vs. angle θ

<p align ="center">
  <img src="./img/RandomAngle.root_NumberOfPEvsTheta.png" width ="500">
</p> 

– Photoelectrons vs. direction of incidence

<p align ="center">
  <img src="./img/RandomAngle.root_NumberOfPEvsAngle.png" width ="500">
</p> 

Findings:
- The angle θ has the greatest impact – the smaller it is, the more photons are generated.
- For angle φ, generation peaks at mid-range values.

### 2.4 Central parallel beam with variable energy

– Photoelectrons vs. proton energy

<p align ="center">
  <img src="./img/RandomEnergy.root_NumberOfPEvsProtonEnergy.png" width ="500">
</p> 

– Average photoelectrons vs. proton energy

<p align ="center">
  <img src="./img/RandomEnergy.root_NumberOfPEvsProtonEnergy1D.png" width ="500">
</p> 

Findings:
- The number of photoelectrons increases with energy.
- Stabilization occurs above approx. 30 GeV.

---

### Table of Contents
- Introduction
- Results
  - Central parallel beam, Ep = 200 GeV
  - Random proton position, Ep = 200 GeV
  - Random angle of incidence, Ep = 200 GeV
  - Variable proton energy
