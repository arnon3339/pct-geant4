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
./examplePCT --output <output path> --phantom <phatom name>
```

The result of runBeam will be generated to /build/output directory for single rotation of phantom.

#### 2.4.2 Batch mode

```text
./examplePCT --macro run_loop.mac --output <output path> --phantom <phatom name>
```

The result of runBeam will be generated to /build/output directory for 0 - 180 degree in phantom ratation.

Phantom names can be use as *.obj* file in phantom directory with out format subfix.

Without running options, the running options will be default values.

## 3. Results

Without using *--output* option, the output data will be in ./output of build directory.

## 4. Configure the simulation

### - Number of primary
  
In *onloop.mac* file

```text
/run/beamOn <number of primary per a projection>
```

### - Rotation angle (degree) of phantom

In run_loop.mac

```text
/control/loop onloop.mac angle <start angle> <end angle> <step> 
```

The rotation of the phantom start rotating from *\<start angle\>* with *\<step\>* step angle until it reachs *\<end angle\>*. The *\<step\>* value can be omitted for stepping with 1 degree. The result will be generated according to the number of rotation steps.

