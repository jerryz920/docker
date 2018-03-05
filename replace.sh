
v1=${1:-v1}

#v1=`docker-machine ip $i`
ssh -tt -i /local/data-population/key1 docker@$v1 'sudo /etc/init.d/docker stop'
#make dynbinary

ssh -tt -i /local/data-population/key1 docker@$v1 'sudo mkdir -p /opt/lib; sudo chown -R docker:docker /opt/lib; '
scp -i /local/data-population/key1 bundles/latest/dynbinary-client/* docker@$v1:/opt/lib/
scp -i /local/data-population/key1 bundles/latest/dynbinary-daemon/* docker@$v1:/opt/lib/
scp -i /local/data-population/key1 /usr/bin/strace docker@$v1:

ssh -tt -i /local/data-population/key1 docker@$v1 'sudo cp /opt/lib/docker-1.14.0-dev /usr/local/bin/docker;
	sudo cp /opt/lib/dockerd-1.14.0-dev /usr/local/bin/dockerd; 
	sudo cp /opt/lib/docker* /usr/local/bin/; 
	sudo cp strace /usr/bin/;
	sudo /etc/init.d/docker start;
	'
