from node:12-buster-slim
RUN apt-get update && apt-get install -y python3 gcc build-essential
WORKDIR app
COPY . .
RUN npm install

CMD npm run start

