# -*- coding: utf-8 -*-
import pyqtgraph as pg
from pyqtgraph.Qt import QtCore, QtGui
import numpy as np
import serial
import serial.threaded
from queue import Queue, Empty
from threading import Thread
import logging

class Filter:
    def __init__(self):
        self.window = []
        self.window2 = []
        self.triggered = 0

    def update(self, z):
        self.window.append(int(z['acz']))
        if len(self.window) > 10:
            self.window = self.window[1:]

        cur = sum(self.window) / len(self.window)
        self.window2.append(cur)
        if len(self.window2) > 30:
            self.window2 = self.window2[1:]

        avg = np.average(np.diff(self.window))

        detected = False
        if avg < -50:
            self.trigger += 1

            if self.trigger >= 8:
                detected = True
        else:
            self.trigger = 0

        return {
            'cur': cur,
            'detected': cur if detected else -10000
        }

class Item:
    def __init__(self, data, curve, value_fn):
        self.data = data
        self.curve = curve
        self.value_fn = value_fn

class Window(pg.GraphicsWindow):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.pause = False
        self.curves = []
        self.filter = Filter()

        self.setup()

        self.Q = Queue()

        def read_lines(Q):
            names = ['time', 'acx', 'acy', 'acz', 'tmp', 'gyx', 'gyy', 'gyz']
            s = serial.Serial('/dev/ttyUSB0', 115200)

            while True:
                try:
                    cols = s.readline().decode('utf-8').strip().split(',')
                    vals = dict(zip(names, cols))
                    vals = {**vals, **self.filter.update(vals)}
                    Q.put(vals)

                    if vals['detected'] != -10000:
                        s.write(b'1')

                
                except Exception as e:
                    logging.exception(e)

        Thread(target=read_lines, args=(self.Q,)).start()

        self.timer = pg.QtCore.QTimer()
        self.timer.timeout.connect(self.update)
        self.timer.start(50)

    def setup(self):
        p1 = self.addPlot()
        p1.setYRange(-16000, 16000)
        self.add_plot(p1, lambda cols: int(cols['acz']), symbol='o')
        self.add_plot(p1, lambda cols: int(cols['detected']), symbol='o', symbolBrush=(255, 0, 0), symbolSize=6)

    def add_plot(self, p, value_fn, **kwargs):
        data = np.random.normal(size=300)
        curve = p.plot(data, **kwargs)
        self.curves.append(Item(data, curve, value_fn))

    def keyPressEvent(self, ev):
        if ev.key() == 32:
            self.pause = not self.pause

    def update(self):
        try:
            while True:
                cols = self.Q.get_nowait()
                
                for item in self.curves:
                    item.data[:-1] = item.data[1:]
                    item.data[-1] = item.value_fn(cols)
        except Empty:
            pass
        except Exception as e:
            print(e)

        if not self.pause:
            for curve in self.curves:
                curve.curve.setData(curve.data)

win = Window()
QtGui.QApplication.instance().exec_()
