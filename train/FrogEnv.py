import gym
from gym import spaces

import numpy as np

from PIL import Image

import sys
sys.path.append(r'../build/python/')

import pyvecx_rl


class FrogEnv(gym.Env):

    def __init__(self, realTime, enableWindow):
        super(FrogEnv,self).__init__()

        self.vecxEnv = pyvecx_rl.environment(frames_per_step = 4,
            real_time = realTime, 
            enable_window = enableWindow,
            enable_sound = False,
            image_dims = (40,60)
        )
        self.vecxEnv.load_rom("frog_jump.bin")

        self.noRewardCounter = 0
        self.stepcounter = 0
        self.done = False

        self.action = pyvecx_rl.action()
        self.legal_actions = self.vecxEnv.get_legal_actions()

        self.action_space = spaces.Discrete(len(self.vecxEnv.get_legal_actions()))
        self.observation_space = spaces.Box(low=0,high=255, shape=(40,60,1),dtype=np.uint8)
    
    def step(self,action):
        self.stepcounter += 1

        #using step function with given action index
        self.action.event = self.legal_actions[action]
        current_reward = self.vecxEnv.step(self.action)        
        
        #counting how long no reward was retrieved
        if current_reward <= 0:
            self.noRewardCounter += 1
        else:
            self.noRewardCounter = 0

        #if no reward has been achieved for the last 40 steps or more the reward becomes negative
        if self.noRewardCounter >= 40:
            current_reward = -1

        #checking if game is done
        if(self.vecxEnv.is_game_finished()):
            current_reward = -10
            self.done = True

        #setting reward
        self.reward = current_reward

        #getting observation
        observation = self.vecxEnv.get_image()
        observation = np.array(observation)
        observation = observation.astype(np.uint8)
        observation = np.reshape(observation, (40,60,1))

        info={}

        print(f"Current Reward: {current_reward} Current Action: {action} Stepcounter: {self.stepcounter}")

        return observation,self.reward,self.done,info
    
    def reset(self):
        self.noRewardCounter = 0
        self.vecxEnv.start_new_game()
        self.reward = 0
        self.done = False
        print("------------RESET----------------")
        observation = self.vecxEnv.get_image()
        observation = np.array(observation)
        observation = observation.astype(np.uint8)
        observation = np.reshape(observation, (40,60,1))
        print(observation.dtype)
        print(observation.shape)
        return observation
       
