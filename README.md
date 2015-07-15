# GeoSmartSim

## Ingelligent Agent platform for spatial simulations built with Qt

### Authors : Ander Pijoan <ander.pijoan(at)deusto.es>, Oihane Kamara <oihane.esteban(at)deusto.es> & Cruz E. Borges <cruz.borges(at)deusto.es>

GeoSmartSim is an agent platform to build geosimulations where situated agents live in an intelligent Agent Environment in order to evaluate different urban dynamics. All actors that interfere in the simulation (including the Agent Environment) are derived from a parent Agent class, containing basic attributes, procedures and space for implementing parallel intelligent behaviours. 

The platform consists of three main pieces:

* Agents

Autonomous Agents constitute the traditional agents in a MAS. They are scheduled to follow a certain behaviour for long periods of time and can interact with other elements or the Environment. When emulating a behaviour, this will be executed in a separate thread using QT's Signal/Slot system to maximize parallelism.

** On the other hand, Passive Entities are those entities who, although having some logic inside, just react to external stimuli. Namely, Passive Entities provide or receive information from agents or the environment and execute simple behaviours as reaction to those actions. 

* Environment (which is also an intelligent agent)

A Physical World and a Social Environment come together to form the universe where both objects and agents are situated. On the on hand, the physycal layer contains spatial indexes and restrictions for storing the agents that inhabit it and on the other, the social layer is in charge of the knowledge, roles and message sending between the agents.

* Skills

Consolidated and robust open source libraries which are in charge of modelling the skills agents are able to execute (including their communication and perception capabilities). Skill sets are the central point of this platform. They represent the degree of competence that empowers agents and the way they perceive or propagate actions. Every agent type will be characterized by the set of skills they are able to use and.



