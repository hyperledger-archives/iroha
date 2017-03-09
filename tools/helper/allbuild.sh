#!/bin/bash
python make_issue_transaction.py $1 account
python make_issue_transaction.py $1 asset
python make_issue_transaction.py $1 peer
python make_issue_transaction.py $1 domain
