from m5stack import *
from m5ui import *
from uiflow import *
import imu
import time
import math
from math import atan, atan2, sqrt
from math import degrees as rad2deg
from m5mqtt import M5mqtt
from easyIO import *


class KalmanAngle:

    def __init__(self):
      
        # Preset configuration by using MPU6050 configuration
        self.QAngle = 0.001
        self.QBias = 0.003
        self.RMeasure = 0.03
        self.angle = 0.0
        self.bias = 0.0
        self.rate = 0.0
        self.P = [[0.0, 0.0], [0.0, 0.0]]
    
    # Get Kalman Angle
    def getAngle(self, newAngle, newRate, dt):
        # step 1:
        self.rate = newRate - self.bias  # new_rate is the latest Gyro measurement
        self.angle += dt * self.rate

        # Step 2: update the matrix with sensor rate and date time
        self.P[0][0] += dt * (dt * self.P[1][1] - self.P[0]
                              [1] - self.P[1][0] + self.QAngle)
        self.P[0][1] -= dt * self.P[1][1]
        self.P[1][0] -= dt * self.P[1][1]
        self.P[1][1] += self.QBias * dt

        # Step 3: Innovation, create a new Y angle
        y = newAngle - self.angle

        # Step 4: Innovation covariance
        s = self.P[0][0] + self.RMeasure

        # Step 5: Update Kalman Gain
        K = [0.0, 0.0]
        K[0] = self.P[0][0] / s
        K[1] = self.P[1][0] / s

        # Step 6: Update the Angle
        self.angle += K[0] * y
        self.bias += K[1] * y

        # Step 7: Calculate estimation error covariance - Update the error covariance
        P00Temp = self.P[0][0]
        P01Temp = self.P[0][1]

        self.P[0][0] -= K[0] * P00Temp
        self.P[0][1] -= K[0] * P01Temp
        self.P[1][0] -= K[1] * P00Temp
        self.P[1][1] -= K[1] * P01Temp

        return self.angle


# Function to convert x y z axis from accelerometer to x and y angle
def acc_angle(Ax, Ay, Az):
  
    # Convert from radian to degree
    radToDeg = 180/3.14159
    # Return the arc tangent of using y axis to generate angle x
    ax_angle = math.atan(Ay/math.sqrt(math.pow(Ax,2) + math.pow(Az, 2)))*radToDeg
    # Return the arc tangent of using x axis to generate angle y
    ay_angle = math.atan((-1*Ax)/math.sqrt(math.pow(Ay,2) + math.pow(Az, 2)))*radToDeg
    return (ax_angle, ay_angle)

def gyr_angle(Gx, Gy, Gz, dt):
    
    gx_angle = Gx*dt + get_last_x_angle()
    gy_angle = Gy*dt + get_last_y_angle()
    gz_angle = Gz*dt + get_last_z_angle()
    return (gx_angle, gy_angle, gz_angle)

  # A complementary filter to determine the change in angle by combining accelerometer and gyro values. 
  # Alpha is the filtered gain and it is depends on the sampling rate...
  # we are setting it as 0.9 by following MPU6050 config
def c_filtered_angle(ax_angle, ay_angle, gx_angle, gy_angle):
    alpha = 0.90
    c_angle_x = alpha*gx_angle + (1.0 - alpha)*ax_angle
    c_angle_y = alpha*gy_angle + (1.0 - alpha)*ay_angle
    return (c_angle_x, c_angle_y)

 # Kalman filter to determine the change in angle by combining accelerometer and gyro values. 
def k_filtered_angle(ax_angle, ay_angle, Gx, Gy, dt):
    k_angle_x = kalmanX.getAngle(ax_angle, Gx, dt)
    k_angle_y = kalmanY.getAngle(ay_angle, Gy, dt)
    return (k_angle_x, k_angle_y)
    

last_read_time = 0.0   
# These are the filtered angles
last_x_angle = 0.0          
last_y_angle = 0.0
last_z_angle = 0.0

def get_last_time(): 
    return last_read_time
def get_last_x_angle():
    return last_x_angle
def get_last_y_angle():
    return last_y_angle
def get_last_z_angle():
    return last_z_angle


kalmanX = KalmanAngle()
kalmanY = KalmanAngle()




setScreenColor(0x111111)
lcd.setRotation(2)

kalman_angle_y = None
receive_topic_data = None
msp_data = None

imu0 = imu.IMU()


power_text = M5TextBox(30, 8, "Power: ", lcd.FONT_Default, 0xFFFFFF, rotate=0)
power_label = M5TextBox(77, 8, "-", lcd.FONT_Default, 0xFFFFFF, rotate=0)

label0 = M5TextBox(30, 37, "label0", lcd.FONT_Default, 0xFFFFFF, rotate=0)
label1 = M5TextBox(30, 66, "label1", lcd.FONT_Default, 0xFFFFFF, rotate=0)
label2 = M5TextBox(30, 92, "label2", lcd.FONT_Default, 0xFFFFFF, rotate=0)
label3 = M5TextBox(30, 120, "label3", lcd.FONT_Default, 0xFFFFFF, rotate=0)
dashboard = M5TextBox(30, 150, "Dashboard", lcd.FONT_Default, 0xFFFFFF, rotate=0)
embedded = M5TextBox(30, 180, "Embedded", lcd.FONT_Default, 0xFFFFFF, rotate=0)

def fun_Dashboard_(topic_data):
  global receive_topic_data, msp_data, uart1
  receive_topic_data = topic_data
  dashboard.setText(str(receive_topic_data))
  uart1.write(str(receive_topic_data))
  pass



calib_x_accel = 0.0 
calib_y_accel = 0.0 
calib_z_accel = 0.0 
calib_x_gyro  = 0.0 
calib_y_gyro  = 0.0 
calib_z_gyro  = 0.0 

m5mqtt = M5mqtt('m5stickc', '192.168.43.90', 1883, 'mqtt', 'asdqwe123', 300)
m5mqtt.subscribe(str('Dashboard'), fun_Dashboard_)
m5mqtt.start()

uart1 = machine.UART(1, tx=32, rx=33)
uart1.init(115200, bits=8, parity=None, stop=1)
while True:
  power_label.setText(str(map_value((axp.getBatVoltage()), 3.7, 4.1, 0, 100)))
  
  # write ccurent time to timestmap
  t_now = time.ticks_ms()
  # print((str('t_now') + str(t_now) ))

  
  # Get the proper time stamp minus the last reading time and convert to second
  dt = (t_now - get_last_time())/1000.0
  
  # print((str('dt') + str(dt) ))

  
  # Format the sensor value

  #accerelometer x y z axis 
  Ax = imu0.acceleration[0]/16384.0
  Ay = imu0.acceleration[1]/16384.0
  Az = imu0.acceleration[2]/16384.0
  
  Gx = (imu0.gyro[0] - calib_x_gyro)/131.0
  Gy = (imu0.gyro[1] - calib_y_gyro)/131.0
  Gz = (imu0.gyro[2] - calib_z_gyro)/131.0
  
  acc_angles = acc_angle(Ax, Ay, Az) # Calculate angle of inclination or tilt for the x and y axes with acquired acceleration vectors
  gyr_angles = gyr_angle(Gx, Gy, Gz, dt) # Calculate angle of inclination or tilt for x,y and z axes with angular rates and dt 
  (c_angle_x, c_angle_y) = c_filtered_angle(acc_angles[0], acc_angles[1], gyr_angles[0], gyr_angles[1]) # filtered tilt angle i.e. what we're after
  (k_angle_x, k_angle_y) = k_filtered_angle(acc_angles[0], acc_angles[1], Gx, Gy, dt)
  
  print( ( str('cx angle') + str(round(c_angle_x, 2)))    )
  # print((str('cy angle') + str(round(c_angle_y, 2))))
  print((str('kx angle') + str(round(k_angle_x, 2))))
  # print((str('ky angle') + str( round(k_angle_y, 2) ) )   )
  
  
  label0.setText(( str('cx') + str(round(c_angle_x, 2)))        )
  label1.setText(( str('cy') + str(round(c_angle_y, 2)))        )
  label2.setText(( str('kx') + str(round(k_angle_x, 2)))        )
  label3.setText(( str('ky') + str(round(k_angle_y, 2))   )        )
  
  # Preset an angle and write to UART if exceed this angle 
  # Angle of elevation
  if(k_angle_x > 28 ):
    uart1.write(str("A")  )
  
  # uart1.write(str(round(k_angle_y, 2))  )

  if uart1.any():
    msp_data = (uart1.readline()).decode()
    embedded.setText(str(msp_data))
    m5mqtt.publish(str('embedded'), str(msp_data), 0)

  wait(1)
  wait_ms(2)
