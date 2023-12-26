from fractions import gcd
import Queue
import sys

HIGH = 'HIGH'
LOW = 'LOW'

ON = "ON"
OFF = "OFF"

def lcm(arr):
    l=reduce(lambda x,y:(x*y)//gcd(x,y),arr)
    return l

class Module():

    def __init__(self, q, modules, name, outputs):
        self.q = q
        self.outputs = outputs
        self.modules = modules
        self.name = name
        self.nb_low = 0
        self.nb_high = 0
        self.last_sent = LOW

    def send_pulse(self, pulse):
        self.last_sent = pulse

        for out_str in self.outputs:
            if pulse == LOW:
                self.nb_low += 1
            elif pulse == HIGH:
                self.nb_high += 1
            else:
                print("wat?")

            out = self.modules[out_str]

            self.q.put((out.receive_pulse, [self, pulse]))

class Button(Module):
    def smash_it(self):
        self.send_pulse(LOW)

class Untyped(Module):
    def __init__(self, *args, **kwargs):
        Module.__init__(self, *args, **kwargs)
        self.received_low_pulse = False
        self.inputs = []

    def receive_pulse(self, sender, pulse):
        if pulse == LOW:
            self.received_low_pulse = True

class Broadcaster(Module):
    def receive_pulse(self, sender, _):
        self.send_pulse(LOW)

class FlipFlop(Module):
    def __init__(self, *args, **kwargs):
        Module.__init__(self, *args, **kwargs)
        self.status = OFF

    def flip(self):
        self.status = ON if self.status == OFF else OFF

    def receive_pulse(self, sender, pulse):
        if pulse == LOW:
            self.flip()

            if self.status == ON:
                self.send_pulse(HIGH)
            else:
                self.send_pulse(LOW)

class Conjunction(Module):
    def __init__(self, *args, **kwargs):
        Module.__init__(self, *args, **kwargs)
        self.inputs = []
        self.input_status = None
        self.is_persist = {}

    def init_status(self):
        self.input_status = {}
        for input in self.inputs:
            self.input_status[input.name] = LOW

    def all_high(self):
        for input_name in self.input_status:
            status = self.input_status[input_name]
            if status == LOW:
                return False

        return True

    def reset_is_persist(self):
        self.is_persist = {}

    def receive_pulse(self, sender, pulse):
        if not self.input_status:
            self.init_status()

        self.input_status[sender.name] = pulse

        if self.name == "bn" and pulse == HIGH:
            self.is_persist[sender.name] = HIGH

        if self.all_high():
            self.send_pulse(LOW)
        else:
            self.send_pulse(HIGH)

def parse_modules(filename):
    fh = open(filename)
    q = Queue.Queue()

    modules = {}
    for line in fh:
        type_name, outputs_str = [x.strip() for x in line.strip().split('->')]
        outputs = [x.strip() for x in outputs_str.split(',')]
        if type_name == "broadcaster":
            modules[type_name] = Broadcaster(q, modules, type_name, outputs)
        else:
            module_type = type_name[0]
            name = type_name[1:]
            if module_type == '%':
                modules[name] = FlipFlop(q, modules, name, outputs)
            else:
                modules[name] = Conjunction(q, modules, name, outputs)

    modules['button'] = Button(q, modules, 'button', ['broadcaster'])

    untyped = {}
    for module_str in modules:
        module = modules[module_str]
        for output in module.outputs:
            try:
                output_module = modules[output]
                inputs = output_module.inputs
            except AttributeError:
                continue
            except KeyError:
                if output not in untyped:
                    untyped[output] = Untyped(q, modules, output, [])

                output_module = untyped[output]

            output_module.inputs.append(module)

    for name, m in untyped.items():
        modules[m.name] = m

    return q, modules, untyped

def push_the_button(q, modules, untyped):
    for key in untyped:
        logic_gate = untyped[key].inputs[0]

    logic_gate.reset_is_persist()
    button = modules['button']

    q.put((button.smash_it, []))
    while q:
        if q.empty():
            break

        method, args = q.get()
        method(*args)

    nb_high = 0
    nb_low = 0
    for module_name, module in modules.items():
        nb_high += module.nb_high
        nb_low += module.nb_low
        
    is_persist = logic_gate.is_persist

    return nb_high, nb_low, is_persist

# q, modules, untyped = parse_modules('input_test_1.txt')
# q, modules, untyped = parse_modules('input_test_2.txt')
q, modules, untyped = parse_modules('input.txt')

button_presses = 0
is_nb = {}
while True:
    if button_presses != 0 and button_presses % 10000 == 0:
        print("Button pressed %s times" % button_presses)

    high_total, low_total, is_persist = push_the_button(q, modules, untyped)
    button_presses += 1
    for key in is_persist:
        pulse = is_persist[key]
        if pulse == HIGH:
            is_nb[key] = button_presses

    if len(is_nb) == 4:
        break

print(is_nb)
print(lcm(is_nb.values()))
