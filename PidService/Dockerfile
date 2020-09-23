FROM python:3.7-slim-buster

RUN apt update && apt install -y gcc
WORKDIR /usr/src/app

COPY . ./
#RUN pip install --no-cache-dir -r requirements.txt
RUN pip install pigpio sqlalchemy pg8000 six py-dateutil

RUN apt install -y g++
RUN pip install pyzmq

