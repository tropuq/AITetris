import tensorflow as tf
import numpy as np

class Learner():
	def __init__(self, lr, inp_size, hidden_size, out_size, gamma=0.99):
		self.simul_states = []
		self.simul_rewards = []
		self.simul_actions = []
		
		self.history_rewards = []
		
		self.gamma = gamma
		
		self.inp = tf.placeholder(tf.float32, shape=[None, inp_size])
		hidden   = tf.layers.dense(self.inp, hidden_size, use_bias=False, activation=tf.nn.softmax)
		self.out = tf.layers.dense(hidden, out_size, use_bias=False, activation=tf.nn.softmax)
		
		self.rewards = tf.placeholder(tf.float32, shape=[None])
		self.actions = tf.placeholder(tf.int32, shape=[None])
		
		indexes = tf.range(0, tf.shape(self.out)[0] * tf.shape(self.out)[1], tf.shape(self.out)[1]) + self.actions
		reshape = tf.reshape(self.out, shape=[-1])
		action_weights = tf.gather(reshape, indexes)
		
		self.loss = -tf.reduce_mean(tf.log(action_weights) * self.rewards)
		
		self.optimize = tf.train.AdamOptimizer(learning_rate=lr).minimize(self.loss)
		
		self.sess = tf.Session()
		self.sess.run(tf.global_variables_initializer())
	
	def __discountRewards(self, r):
		ret = np.zeros_like(r)
		sm = 0.
		for i in reversed(range(ret.size)):
			sm = self.gamma * sm + r[i]
			ret[i] = sm
		return ret - np.mean(ret)
	
	def nextAction(self, state):
		self.simul_states.append(state)
		
		action = self.sess.run(self.out, feed_dict={self.inp: [state]})
		action = np.random.choice(range(action[0].size), p=action[0])
		
		self.simul_actions.append(action)
		
		return action
	
	def addReward(self, reward):
		self.simul_rewards.append(reward)
	
	def episodeDone(self):
		self.sess.run(self.optimize, feed_dict={self.rewards: self.__discountRewards(self.simul_rewards),
		                                   self.actions: self.simul_actions,
		                                   self.inp:     self.simul_states})
		
		self.history_rewards.append(np.sum(self.simul_rewards))
		
		self.simul_rewards = []
		self.simul_actions = []
		self.simul_states = []
