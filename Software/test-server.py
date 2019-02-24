import http.server
import socketserver

PORT = 1337

class Handler(http.server.BaseHTTPRequestHandler):
    def do_GET(self):
        print(self.path[1:] + " deg C")
        self.send_response(200)

with socketserver.TCPServer(("", PORT), Handler) as httpd:
    print("Serving at port", PORT)
    httpd.serve_forever()