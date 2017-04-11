# EPL371as4


############################################################################################
#######################################Contributors#########################################

Andreas Charalampous
Modestos Ioannou
Chrystalla Chrysostomou

############################################################################################





############################################################################################

Instructions on how to use the server are provided in this README file.

############################################################################################

You can compile the program using "make" into terminal.Config.json file includes the parameters
for using the program. It contains the port, the number of threads and the source folder.
All these parameters can change at any time by editing the file .

After configuring the config.json file you can "run" the server by executing the "./s" command 
into terminal, then you can open any browser (Mozilla Firefox , Google Chrome , Safari)
and make a GET request. At address bar you shall write " localhost:<port>/<file>"

e.g localhost:8080/index.html

You can open multiple tabs and make multiple GET requests as described above.

You can use telnet command to make HEAD and DELETE requests.

You can terminate the server with ctrl-c keyboard combination

