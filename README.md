# curi_udp

## How to use
### 1. Initial the communication
+ Using **communication_init** function to initial and start the communication
```
communication_init(local_ip, local_port, remote_ip, remote_port)
```
### 2. Send and receive data
+ All the data are transfered in string data type and code/decode by using two functions (which you may change in your applications):
```
communication_pack()
communication_unpack()
```
+ Send data by using **communication_send** function
```
communication_send()
```
+ Blocking (waiting) for receive data by using **communication_receive** function
```
communication_receive()
```
+ Non-blocking for receive data by using **communication_get** function
```
communication_get(waiting_time_in_us)
```
### 3. Close the communication
+ Using **communication_init** function to close the communication
```
communication_close()
```
