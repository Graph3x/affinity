from fastapi import FastAPI, Form
from typing import Annotated
import os
import uuid

app = FastAPI()

I = 0
N = 0


@app.post("/")
async def posted(teplota: Annotated[str, Form()] = "None", vlhkost: Annotated[str, Form()] = "None", tlak: Annotated[str, Form()] = "None", souradnice: Annotated[str, Form()] = "None", vyska: Annotated[str, Form()] = "None", akcelerometr: Annotated[str, Form()] = "None"):
    global I
    data = {
        "tep": teplota,
        "vl": vlhkost,
        "tl": tlak,
        "gps": souradnice,
        "vys": vyska
    }

    jdata = f"{teplota}::{vlhkost}::{tlak}::{souradnice}::{vyska}"
    with open('data.cj', 'a+') as f:
        f.write(f"{I} = {jdata}\n")

    I += 1

    return "OK"


@app.post("/tracker")
async def posted_tracker(souradnice: Annotated[str, Form()] = "None", vyska: Annotated[str, Form()] = "None"):
    global N
    data = {
        "gps": souradnice,
        "vys": vyska
    }

    jdata = f"{souradnice}::{vyska}"
    with open('tracker.cj', 'a+') as f:
        f.write(f"{N} = {jdata}\n")

    N += 1

    return "OK"

a = """
@app.get("/")
async def root():
    with open("data.cj", "rb") as f:
        try:
            f.seek(-2, os.SEEK_END)
            while f.read(1) != b'\n':
                f.seek(-2, os.SEEK_CUR)
        except OSError:
            f.seek(0)
        last_line = f.readline().decode()

    return last_line
"""


@app.get("/")
async def root():
    return "hello"


@app.get("/tracker")
async def tracker():
    with open("tracker.cj", "rb") as f:
        try:
            f.seek(-2, os.SEEK_END)
            while f.read(1) != b'\n':
                f.seek(-2, os.SEEK_CUR)
        except OSError:
            f.seek(0)
        last_line = f.readline().decode()

    return last_line


# @app.post("/custom")
# async def custom(data: str, file: str):

#    rand = uuid.uuid1()

#    if file == "tracker":
#        with open('tracker.cj', 'a+') as f:
#            old = f.readlines()
#        with open(f'tracker{rand.hex}.cj.bak', 'w+') as f:
#            f.write(old)

#        with open('tracker.cj', 'w') as f:
#            f.write(data)

#    return "OK"
