from fastapi import FastAPI, Request
import json

app = FastAPI()


# INTER ENDPOINT GLOBALS
trac_data = {
        "gps": "NaN",
        "speed": "NaN",
        "padak": "0",
        "magnet": "0"
    }

Launched = False


def struct_module(data: list) -> dict:

    op_code = 1
    global Launched
    if not Launched:

        if trac_data["magnet"] == "1":
            op_code = 2
        elif trac_data["magnet"] != "0":
            op_code = 3
            Launched = True
    else:
        op_code = 4
        if trac_data["padak"] != "0":
            op_code = 5
    
        if float(data["speed"]) < 3:
            op_code = 6


    return{
        "time": data[0],
        "global_time": data[2],
        "gps": data[3],
        "speed": data[4],
        "height": data[5],
        "acc_x": data[6],
        "acc_y": data[7],
        "acc_z": data[8],
        "temp": data[9],
        "pressure": data[10],
        "humid": data[11],
        "op_code": 1,
        "mag": data["magnet"],
        "gp2": data["gps"],
        "padak": data["padak"]
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

    sdata = sus_data.split('a')
    global trac_data
    trac_data = {
        "gps": sdata[0],
        "speed": sdata[1],
        "padak": sdata[2],
        "magnet": sdata[3]
    }

    print(json.dumps(trac_data))

    return "OK"


@app.get("/")
async def root():
    return "OK"
