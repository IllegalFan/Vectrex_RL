from FrogEnv import FrogEnv
from stable_baselines3 import PPO
import gym


env = FrogEnv(True, True)
env.reset()

#set path to trained model here
model_path = "/home/illegalfan/Programming/Vectrex/repo2/Vectrex_RL/example/python-interface/models/08_11_2022_23_14_30/2520000.zip"

model = PPO.load(model_path, env=env)

for ep in range(500):
    obs = env.reset()
    done = False
    while not done:
        action, _states = model.predict(obs)
        obs, rewards, done, info = env.step(action)
        print(rewards)


