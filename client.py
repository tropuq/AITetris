from window_manager import WindowManager
from ai import Learner
import numpy as np


class Manager:
	def __init__(self): 
		self.actions = ["Right", "Left", "space", "Up"] 
		self.ai = Learner(lr=1e-2, inp_size=400, hidden_size=40, out_size=len(self.actions))
		self.wind = WindowManager()
		
	def run(self):
		while True:
			oper = input()
			assert oper == "STATE" or oper == "REWARD" or oper == "END"
			if oper == "STATE":
				state = np.concatenate([list(map(float, input().split())) for i in range(40)])
				ac = self.ai.nextAction(state)
				self.performAction(ac)
			elif oper == "REWARD":
				tm = input()
				reward = float(tm)
				# if reward != 0:
				# 	print(reward, tm)
				self.ai.addReward(reward)
			elif oper == "END":
				self.ai.episodeDone()
				print(self.ai.history_rewards[-1])
				# self.performAction("space") # new game
	
	def performAction(self, num):
		self.wind.tap(self.actions[num])
	

man = Manager()

man.run()

# x = WindowManager()