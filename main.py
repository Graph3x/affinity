from fastapi import FastAPI, Request

app = FastAPI()

I = 0
N = 0


@app.post("/")
async def posted(request: Request):

    print(request.headers['content-type'])

    return "OK"


@app.post("/tracker")
async def posted_tracker(request: Request):

    print(request.headers['content-type'])

    return "OK"


@app.get("/")
async def root():
    return "hello"
