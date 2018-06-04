from redmine:latest

COPY ./plugins/  /usr/src/redmine/plugins/ 

RUN apt update

RUN apt upgrade -y; apt install zip unzip -y 

ENV RAILS_ENV production

RUN cd /usr/src/redmine;  bundle update;
RUN bundle install --without development test; 

RUN bundle install --without development test; \
	for adapter in mysql2 postgresql sqlserver sqlite3; do \
		echo "$RAILS_ENV:" > ./config/database.yml; \
		echo "  adapter: $adapter" >> ./config/database.yml; \
		bundle install --without development test; \
		cp Gemfile.lock "Gemfile.lock.${adapter}"; \
	done; \
	rm ./config/database.yml;

ENTRYPOINT ["/docker-entrypoint.sh"]

EXPOSE 3000

CMD ["rails", "server", "-b", "0.0.0.0"]