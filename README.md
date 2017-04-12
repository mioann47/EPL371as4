# EPL371as4

## Contributors
```
Andreas Charalampous achara28@cs.ucy.ac.cy
Modestos Ioannou     mioann47@cs.ucy.ac.cy
Chrystalla Chrysostomou

```
See also the list of [contributors](https://github.com/mioann47/EPL371as4/contributors) who participated in this project.


### Instructions on how to use the server are provided in this README file.
You can compile the program using "make" into terminal. Config.json file includes the parameters
for executing the program. It contains the port, the number of threads and the server folder (where the server files are stored).
All these parameters can change at any time by editing the file before running the server.

After configuring the config.json file you can "run" the server by executing the "./s" command 
into terminal, then you can open any browser (Mozilla Firefox , Google Chrome , Safari)
and make a GET request. At address bar you shall write " localhost:<port>/<file>"


```
e.g localhost:8080/index.html

```

You can open multiple tabs and make multiple GET requests as described above.

You can use telnet command to make HEAD and DELETE requests.

You can terminate the server with ctrl-c keyboard combination

You can compile the test file by using "make test" into terminal and then execute it by "./TestFile" command
to see the test results.
## License

This project is licensed under the Apache License - see the [LICENSE.txt](LICENSE.txt) file for details
