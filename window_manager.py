import subprocess

class WindowManager:
	def __init__(self):
		self.selectWindow()
	
	def selectWindow(self):
		self.winid = int(subprocess.check_output(["xdotool", "selectwindow"]))
	
	def activateWindow(self):
		subprocess.call(["xdotool", "windowactivate", str(self.winid)])
	
	def type(self, s):
		"""
		Example: type("ls")
		"""
		self.activateWindow()
		subprocess.call(["xdotool", "type", "--delay", "0", s])
	
	
	def tap(self, code):
		"""
		Example: tap("Return"), tap("ctrl+c")
		"""
		self.activateWindow()
		subprocess.call(["xdotool", "key", "--delay", "0", code])
