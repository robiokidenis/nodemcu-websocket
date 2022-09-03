# API Dockerfile
FROM node:10
USER root
WORKDIR /usr/src/app
COPY package*.json ./

#
RUN npm install
COPY . .
EXPOSE 4001
CMD ["npm", "start"]