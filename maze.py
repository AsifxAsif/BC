import socket
import time

# Define the host and port for the connection
host = '178.128.214.190'
port = 7777

try:
    # Create the socket connection
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect((host, port))

    # Send team code to access the maze
    client_socket.sendall(b'Cyber Samurai\n')
    time.sleep(1)

    # Read data from server and parse maze
    maze_data = client_socket.recv(4096).decode()
    print(maze_data)  # Print initial maze layout

    # Predefined moves sequence
    moves = [
        'd', 'd', 'd', 'd', 'd', 'd', 'd', 'd', 'r',
        'r', 'r', 'r', 'd', 'd', 'd', 'd', 'r', 'r',
        'r', 'r', 'r', 'r', 'u', 'u', 'r', 'r', 'r',
        'r', 'r', 'r', 'r', 'r', 'r', 'r', 'd', 'd', 
	'r', 'r', 'r', 'r', 'u', 'u', 'r', 'r', 'u', 
	'u', 'u', 'u', 'r', 'r', 'd', 'd', 'r', 'r', 
	'd', 'd', 'r', 'r', 'd', 'd', 'r', 'r', 'u', 
	'u', 'r', 'r', 'd', 'd', 'd', 'd', 'd', 'd', 
	'd', 'r'
    ]

    # Execute moves based on the predefined sequence
    for move in moves:
        print(f"Sending move: {move}")
        client_socket.sendall((move + '\n').encode())
        time.sleep(0.1)  # Small delay to allow server response
        
        # Receive and print feedback
        feedback = client_socket.recv(4096).decode()
        print(feedback)

except Exception as e:
    print(f"An error occurred: {e}")
finally:
    # Close the socket
    client_socket.close()
