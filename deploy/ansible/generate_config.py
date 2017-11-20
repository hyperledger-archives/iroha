import optparse
from pathlib import Path


home = str(Path.home())

parser = optparse.OptionParser()

parser.add_option('-p', '--postgres-password',
                  help="postgress password")

parser.add_option('-u', '--postgres-user',
                  help="postgress user")


options, args = parser.parse_args()

if not options.postgres_password or not options.postgres_user:
    raise ValueError("No user or pass provided")


with open(home+"/iroha_data/config.sample", "w") as f:
    s = """{{
  "block_store_path" : "/tmp/block_store/",
  "torii_port" : 50051,
  "internal_port" : 10001,
  "pg_opt" : "host=iroha_postgres port=5432 user={} password={}",
  "redis_host" : "iroha_redis",
  "redis_port" : 6379,
  "max_proposal_size" : 10,
  "proposal_delay" : 5000,
  "vote_delay" : 5000,
  "load_delay" : 5000
}}""".format(options.postgres_user, options.postgres_password)

    f.write(s)
