import sys
import queue

HIGH = 'HIGH'
LOW = 'LOW'

ON = "ON"
OFF = "OFF"

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
            print(self.name, pulse, '->', out.name);

            self.q.put((out.receive_pulse, [self, pulse]))

class Button(Module):
    def smash_it(self):
        self.send_pulse(LOW)

class Untyped(Module):
    def receive_pulse(self, sender, pulse):
        pass

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

    def receive_pulse(self, sender, pulse):
        if not self.input_status:
            self.init_status()

        self.input_status[sender.name] = pulse

        if self.all_high():
            self.send_pulse(LOW)
        else:
            self.send_pulse(HIGH)

def parse_modules(filename):
    fh = open(filename)
    q = queue.Queue()

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

    to_add = []
    for module_str in modules:
        module = modules[module_str]
        for output in module.outputs:
            try:
                output_module = modules[output]
                inputs = output_module.inputs
            except AttributeError:
                continue
            except KeyError:
                to_add.append(Untyped(q, modules, output, []))
                continue

            output_module.inputs.append(module)

    for m in to_add:
        modules[m.name] = m

    return q, modules

def push_the_button(q, modules):
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

    return nb_high, nb_low

# q, modules = parse_modules('input_test_1.txt')
# q, modules = parse_modules('input_test_2.txt')
q, modules = parse_modules('input.txt')

for n in range(1000):
    high_total, low_total = push_the_button(q, modules)

print(high_total)
print(low_total)
print(high_total * low_total)

