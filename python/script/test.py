import tapebackup

if __name__ == "__main__":
    tapebackup.write("/home/leaf/test", "/home/leaf/dummy.img", verbose=True)
    tapebackup.read(".", "/home/leaf/dummy.img", verbose=True)
    tapebackup.check("/home/leaf/dummy.img", verbose=True)
