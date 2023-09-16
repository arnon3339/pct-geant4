# pCT GEANT4

---

Geant4 - an Object-Oriented Toolkit for Simulation in HEP

## 1. Requrements

- Gean4 version 11.0
- CLEP
- ROOT
- CMAKE version in minimum (3.16 - 3.21)

## 2. Execute program

### 2.1 Go to app directory

```text
cd yourpath/pct-geant4
```

### 2.2 Create build directory inside the app and move to it

```text
mkdir build && cd build
```

### 2.3 CMAKE and make it with number of threads

```text
cmake .. && make -j<number of threads>
```

### 2.4 Run the simulation

#### 2.4.1 Vis mode

```text
./examplePCT
```

The result of runBeam will be generated to /build/output directory for single rotation of phantom.

#### 2.4.2 Batch mode

```text
./examplePCT run_loop.mac <run number>
```

The result of runBeam will be generated to /build/output directory for 0 - 90 degree in phantom ratation.

- The *<run number\>* is an optional. If you speficy it, it will create result directory corresponding to that number.

## 3. Configure the simulation

### - Number of primary
  
In onloop.mac file

```text
/run/beamOn <number of primary per a projection>
```

### - Rotation angle (degree) of phantom

In run_loop.mac

```text
/control/loop onloop.mac angle <start angle> <end angle> <step> 
```

The rotation of the phantom start rotating from *\<start angle\>* with *\<step\>* step angle until it reachs *\<end angle\>*. The result will be generated according to the number of rotation steps.

***Please not that:*** The *\<step\>* has to be greater than 1 degree. 