#************************************************************************************************************
# Simple example of the usage of the python interface with random inputs instead of a machine learning agent
#************************************************************************************************************

from random import randint

import sys
sys.path.append(r'../../build/python/')

import pyvecx_rl

if __name__ == "__main__":
    image_dims : tuple = (40, 60)
    pixels : list = []

    env = pyvecx_rl.environment(frames_per_step = 25,
        real_time = True,
        enable_window = True,
        enable_sound = True,
        image_dims = image_dims
    )

    cartridge = "frog_jump.bin"

    try:
        env.load_rom(cartridge)
    except RuntimeError as re:
        print(re)
        exit(-1)

    action = pyvecx_rl.action()
    reward : int = 0

    legal_actions : list = env.get_legal_actions()

    for episode in range(0, 10):
        env.start_new_game()

        while(not env.is_game_finished()):
            action.event = legal_actions[randint(0,9) % len(legal_actions)]

            reward += env.step(action)

            pixels = env.get_image()

            # feed reward and image to a reinforcement learning agent ...

        print("Episode: {}, Score: {}".format(episode, reward))
        reward = 0
