from fastapi import FastAPI, Request
import json

app = FastAPI()


def struct_module(data: list) -> dict:
    return{
        "run_time": data[0],
        "global_time": data[2],
        "gps": data[3],
        "speed": data[4],
        "height": data[5],
        "acc_x": data[6],
        "acc_y": data[7],
        "acc_z": data[8],
        "temp": data[9],
        "press": data[10],
        "humid": data[11]
    }


@app.post("/")
async def posted(request: Request):

    sus_data = request.headers['content-type']
    data = struct_module(sus_data.split("|"))

    dat = json.dumps(data)
    print(dat)

    with open("data.cj", "a") as f:
        f.write(dat)
        f.write("\n")

    return "OK"


@app.post("/tracker")
async def posted_tracker(request: Request):

    sus_data = request.headers['content-type']

    data = {
        "gps": sus_data.split(', Speed (m/s):')[0],
        "speed": sus_data.split(', Speed (m/s):')[1]
    }

    print(json.dumps(data))

    return "OK"


@app.get("/")
async def root():
    return "OK"
