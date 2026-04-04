from flask import Flask, request, send_file 
import socket

app = Flask(__name__) 

@app.route('/')
def index():
    return send_file('control.html')

@app.route('/cmd/<path:command>')
def cmd(command):
    print(f"Command received: {command}")
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect(('127.0.0.1', 8080))
        sock.send(command.encode())
        response = sock.recv(1024).decode()
        sock.close()
        print(f"Response: {response}")
        return response 
    except Exception as e:
        print(f"Error: {e}")
        return f"Error: {e}"

if __name__ == '__main__':
    print("Web panel starting at http://127.0.0.1:5000")
    print("Make sure C++ server is running!")
    app.run(port=5000, debug=True)
    