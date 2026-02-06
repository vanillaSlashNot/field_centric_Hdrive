# VEX V5 SDK to PROS EZ Template Conversion Guide

## Changes Made

Your code has been converted from VEX V5 SDK to PROS with EZ Template framework. Here's what was changed:

### 1. **Headers & Includes** (`include/main.h`)
- Replaced VEX SDK includes with PROS API headers (`api.h`)
- New header file `main.h` replaces `vex.h`

### 2. **Device Declarations** (in `src/main.cpp`)
- **Motors**: Changed from `vex::motor` to `pros::Motor`
  - Port numbers: No longer use PORT constants (use integers directly)
  - Reversed status: Now second parameter (boolean)
  - Gear ratio: Use `pros::MotorGears::green` instead of `vex::gearSetting::ratio36_1`

- **IMU**: Changed from `vex::inertial` to `pros::IMU`

- **Removed**: Motor groups (use individual motors or create custom groups)

### 3. **API Changes**

| VEX V5 SDK | PROS |
|-----------|------|
| `Controller.Axis3.value()` | `controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y)` |
| `Controller.Axis1.value()` | `controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X)` |
| `Controller.Axis4.value()` | `controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X)` |
| `imu1.isCalibrating()` | `imu1.is_calibrating()` |
| `imu1.heading()` | `imu1.get_heading()` |
| `motor.velocity(rpm)` | `motor.get_actual_velocity()` |
| `motor.spin(forward, value, rpm)` | `motor.move(value)` |
| `thread t = thread(func)` | `pros::Task t(func)` |
| `this_thread::sleep_for(ms)` | `pros::delay(ms)` |
| `Brain.Screen.printAt()` | `pros::lcd::print()` |

### 4. **Task Functions**
- Renamed from thread functions to task functions
- Changed return type from `int` to `void`
- `fieldCentrixMecanumDrive()` → `fieldCentricMecanumDriveTask()`
- `motorDebugThread()` → `motorDebugTask()`

### 5. **Main Function**
- Removed `vexcodeInit()` call (not needed in PROS)
- Changed `thread` to `pros::Task`
- Updated motor control to use `motor.move()` with integer values (0-127)

## Next Steps to Complete Setup

### Option A: Using PROS CLI (Recommended)
1. Install [PROS CLI](https://pros.cs.purdue.edu/)
2. Open the project: `prosv5 conduct`
3. This will generate the proper `project.pros` configuration file and remaining build files

### Option B: Manual Setup
1. Create `project.pros` in the root directory:
```json
{
  "py/object": "pros.conductor.project.Project",
  "py/state": {
    "name": "field_centric_Hdrive",
    "target": "v5",
    "templates": {},
    "upload_options": {}
  }
}
```

2. Install PROS headers if not already present
3. Use `prosv5 make` to build

## Motor Port Mapping
Your current code uses ports:
- Left Front: 1, 3 (reversed)
- Left Back: 2, 11 (reversed on port 2 only)
- Right Front: 10, 8 (reversed on port 8)
- Right Back: 9, 20 (reversed on port 20)

## Important Notes
- PROS uses 0-127 for motor speeds (normalized from controller input)
- The normalization code keeps max speed at 100
- IMU calibration is automatic but you can check with `is_calibrating()`
- LCD print uses line numbers (0-7 typically) instead of X,Y coordinates
