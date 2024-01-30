import mido
import serial
import threading
import time

def play_note(note):
    midi_out.send(mido.Message('note_on', note=note, velocity=127))
    time.sleep(0.25)
    midi_out.send(mido.Message('note_off', note=note, velocity=127))

serial_in = serial.Serial('COM9', baudrate=115200)
midi_out = mido.open_output('loopMIDI Port 1')

try:
    while True:
        note = int(serial_in.readline().rstrip())
        threading.Thread(target=play_note, args=(note,)).start()
except KeyboardInterrupt:
    midi_out.close()
