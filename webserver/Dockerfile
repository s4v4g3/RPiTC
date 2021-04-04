from node:lts-buster-slim
RUN apt-get update && apt-get install -y python3
WORKDIR app
COPY . .
RUN npm install

CMD npm run start

