all: build test

build:
	cargo build --release

test:
	cargo test

pyenv:
	pipenv install --dev
