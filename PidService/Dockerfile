FROM python:3.7-slim-buster

WORKDIR /usr/src/app
RUN apt update && apt install -y gcc g++ && pip install pyzmq

COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt
COPY . ./



