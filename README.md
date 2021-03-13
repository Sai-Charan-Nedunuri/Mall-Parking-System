# Mall Parking System

 A parking management system automates a car parking process. It optimizes parking space and makes processes efficient. It gives real-time car parking information such as vehicle & slot counts, available slots, reserved parking.
 
## :key: Key Features
Tasks - Real-time allocation of the parking spot for a fixed duration of time. The incoming tasks     i.e. – vehicles are scheduled via FIFO based on their arrival times and parking time (execution time).

Resources - Parking spots are the resources. We will use counting semaphores to check the status of parking spots whether they are occupied or not.

Initial Assumptions - We assume that the number of tasks (vehicles) competing for resources (parking spots)  are 10, and the number of resources are 5. 

## 📈: Flow of Application
Cars are randomly occurring and being allotted a parking spot as per their availability.   For the real time application experience and efficiently utilizing the features of FreeRTOS (like scheduling and counting semaphores) we have added random delays for each car task so that only when there is free parking spot is available then only the car which is currently available and had low parking time will be allocated.

![image](https://user-images.githubusercontent.com/75213715/111024169-3a21e400-8403-11eb-8985-7d59a2403eae.png)


