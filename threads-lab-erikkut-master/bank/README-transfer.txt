By moving the mutex locks to outside of the loop I was able to avoid the
program deadlock.
