from FrogEnv import FrogEnv
from stable_baselines3 import DQN,PPO
import gym


env = FrogEnv(True, True)
env.reset()

#set path to trained model here


model_path = "/home/illegalfan/Programming/Vectrex/repo2/Vectrex_RL/train/models/20_11_2022_12_52_28/2140000.zip"
model = PPO.load(model_path, env=env)

#model_path = "/home/illegalfan/Programming/Vectrex/repo2/Vectrex_RL/train/models/18_11_2022_12_50_29/2680000.zip"
#model = DQN.load(model_path, env=env)

for ep in range(500):
    obs = env.reset()
    done = False
    while not done:
        action, _states = model.predict(obs)
        obs, rewards, done, info = env.step(action)
        print(rewards)


