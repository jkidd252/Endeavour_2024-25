# CanSat Mach 25 Competition

Welcome to the CanSat project repository for the Mach 25 Competition!

## Overview

This repository contains the codebase for the CanSat being flown at the Mach 25 Competition. The CanSat is deployable and includes piezo electric experiment on ascent and descent. The CanSat will turn into a rover once it hits the ground and while begin sampling the air.

## Repository Structure

- `src/`: Contains the source code for the CanSat.
    - `recovery/`: Contains the recovery simulations and code.
    - `telemetry/`: Contains the code for telemetary sensors.
    - `piezo/`: Contains the code for the piezo experiments.
        - `ml/`: Contains the code for the piezo-AI integration.
    - `rover/`: Contains the code for the rover.
    - `brain/`: Contains the the sequencing and microcontroller communication code.
- `tests/`: Test scripts and data for validating the CanSat's functionality.
- `docs/`: Documentation on failure modes, code reviews etc.

## Usage

Payload peeps, pls keep all your code here and in the relevant folders! 

## Contributing

We welcome contributions!

---

Happy coding!
