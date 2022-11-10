from stable_baselines3.common.env_checker import check_env
from FrogEnv import FrogEnv

from stable_baselines3 import PPO

from datetime import datetime

now = datetime.now()
dt_string = now.strftime("%d_%m_%Y_%H_%M_%S")

env = FrogEnv(False, False)

models_dir = f"models/{dt_string}"
logdir = f"logs/{dt_string}"

model = PPO('MlpPolicy', env, verbose=1, tensorboard_log=logdir)

TIMESTEPS = 20000
iters = 0
while True:
	iters += 1
	model.learn(total_timesteps=TIMESTEPS, reset_num_timesteps=False, tb_log_name=f"PPO")
	model.save(f"{models_dir}/{TIMESTEPS*iters}")