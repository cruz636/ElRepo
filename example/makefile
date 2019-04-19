compiler:
	rm -rf client server pthreadServer logS logC 
	gcc -o "server" ./server.c -lcommons -lcompartidas
	gcc -o "client" ./client.c -lreadline -lcommons -lcompartidas
	gcc -o "pthreadServer" ./pthreadServer.c -lpthread -lcommons -lcompartidas