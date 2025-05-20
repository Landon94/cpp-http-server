import unittest
import time
import requests
import signal
import subprocess


class TestServer(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        subprocess.run(["make"], check=True)
        cls.server = subprocess.Popen(["./main"])
        time.sleep(1)
    
    @classmethod
    def tearDownClass(cls):
        cls.server.send_signal(signal.SIGINT)
        cls.server.wait()
        subprocess.run(["make", "clean"])

    def test_root(self):
        response = requests.get("http://localhost:8080/")
        self.assertEqual(response.status_code,200)

    def test_user_agent(self):
        headers = {
            "User-Agent": "test/foo"
        }
        response = requests.get("http://localhost:8080/user-agent", headers=headers)
        self.assertEqual(response.status_code,200)
        self.assertEqual(response.text,"test/foo\r")

    def test_echo(self):
        response = requests.get("http://localhost:8080/echo/test")
        self.assertEqual(response.status_code,200)
        self.assertEqual(response.text,"test")

    def test_404(self):
        response = requests.get("http://localhost:8080/DoesNotExist")
        self.assertEqual(response.status_code,404)

if __name__ == '__main__':
    unittest.main()