from flask import Flask, request, send_file 
import socket

# Create Flask application instance
app = Flask(__name__) 

# Route for the main page - servers the HTML control panel
@app.route('/')
def index():
    # Send the HTML file to the browser
    return send_file('control.html')

# Route that handles commands sent from the page
# <path:command> captures the entire command string (including slashes)
@app.route('/cmd/<path:command>')
def cmd(command):
    # Print to termainl for debugging - shows which command was received
    print(f"Command received: {command}")
    try:
        # Create a TCP socket (same as C++ client)
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        # Connect to C++ server on localhost port 8080
        sock.connect(('127.0.0.1', 8080))
        # send the command to C++ server (must be bytes)
        sock.send(command.encode())
        # wait for receive response from C++ server (1024 bytes max)
        response = sock.recv(1024).decode()
        # close the connection
        sock.close()
        # print response to terminal for debugging
        print(f"Response: {response}")
        # return response to browser 
        return response 
    except Exception as e:
        # If anything fails (server not running, connection refused, etc.)
        print(f"Error: {e}")
        return f"Error: {e}"
# Run the flask server when script is executed directly
if __name__ == '__main__':
    print("Web panel starting at http://127.0.0.1:5000")
    print("Make sure C++ server is running!")

    # Start Flask on port 5000, debug=True auto-reloads on code changes
    app.run(port=5000, debug=True)
    
